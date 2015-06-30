#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/fs/zfs.h>
#include <libzfs.h>
#include <string.h>

#include "fsutil.h"
#include "config.h"
#include "stdprint.h"
#include "vdev_status.h"
#include "zio.h"
#include "memlist.h"

/*
 * NOTE: libzfs is an unstable interface. 
 * This module may or may not work on your illumos distribution/version.
 * Compile with: gcc -Wall -Wextra -std=c99 -pedantic -lzfs -lnvpair zio.c config.c fsutil.c stdprint.c -o zio
 */


int
zpool_get_stats(zpool_handle_t * zhp, void * data) {
	config_t * cnf = (config_t *)data;
	uint_t c;
    boolean_t missing;

	nvlist_t * nv, * config;
	vdev_stat_t * vs;

    if (zpool_refresh_stats(zhp, &missing) != 0)
		return 1;

	config = zpool_get_config(zhp, NULL);

	if (nvlist_lookup_nvlist(config,
                ZPOOL_CONFIG_VDEV_TREE, &nv) != 0) {
                return 1;
        }

        if (nvlist_lookup_uint64_array(nv,
                ZPOOL_CONFIG_VDEV_STATS, (uint64_t **)&vs, &c) != 0) {
                return 1;
        }

	if (!strcmp(zpool_get_name(zhp), cnf->zname)) {
		cnf->zpool.read_ops  = vs->vs_ops[ZIO_TYPE_READ];			
		cnf->zpool.write_ops = vs->vs_ops[ZIO_TYPE_WRITE];
		cnf->zpool.read_bts  = vs->vs_bytes[ZIO_TYPE_READ];
		cnf->zpool.write_bts = vs->vs_bytes[ZIO_TYPE_WRITE];
		cnf->zpool.alloc = vs->vs_alloc;
		cnf->zpool.free = vs->vs_space - vs->vs_alloc;
		cnf->zpool.health = zpool_get_health(zhp);
		cnf->zpool.dedupratio = zpool_get_dedupratio(zhp);
		cnf->zpool.name = zpool_get_poolname(zhp);
	}
	

	zpool_close(zhp);
	return 0;
}

int
zfs_get_stats(zfs_handle_t * zhf, void * data) {
	config_t * cnf = (config_t *)data;

	if (!strcmp(zfs_get_name(zhf), cnf->zname)) {
		cnf->zfs.used = zfs_get_used(zhf);			
		cnf->zfs.compressratio = zfs_get_compressratio(zhf);
		cnf->zfs.available = zfs_get_available(zhf);
		cnf->zfs.logical = zfs_get_logical(zhf);
		cnf->zfs.name = zfs_get_fsname(zhf);
	}

	zfs_close(zhf);
	return 0;
}


int
main(int argc, char *argv[]) {
	config_t cnf;
	devlist_t d;

	cnf.zname[0] = '\0';

	g_zfs = libzfs_init();

	init_config(&cnf);
	init_devlist(&d);

	get_config(argc, argv, &cnf);

	zpool_iter(g_zfs, zpool_get_stats, (void *)&cnf);
	zfs_iter_root(g_zfs, zfs_get_stats, (void *)&cnf);
	zpool_iter(g_zfs, zpool_print_vdev, (void *)&d);

	if (cnf.sw == SW_UNDEF) {
		fprintf(stderr, "show type is not defined\n");

		return 1;
	} else if (cnf.sw == SW_POOLS) {
		if (cnf.ft == TP_UNDEF) {
			fprintf(stderr, "undef format type\n");
			return 1;
		}
		else if (cnf.ft == TP_TEXT) show_zpools(g_zfs);
		else if (cnf.ft == TP_JSON) show_zpools_json(g_zfs);	

		return 0;
	} else if (cnf.sw == SW_DEVSTATE) {
		find_state_in_devlist(&d, cnf.vdev);
		free_devlist(&d);

		return 0;
	} else if (cnf.sw == SW_DEVICES) {
		if (cnf.ft == TP_UNDEF) {
			fprintf(stderr, "undef format type\n");
			return 1;
		}
		else if (cnf.ft == TP_TEXT) print_devlist_text(&d);
		else if (cnf.ft == TP_JSON) print_devlist_json(&d);

		return 0;
	}

	if(cnf.zpool.name == NULL || cnf.zfs.name == NULL) {
		fprintf(stderr, "could not find zpool: %s\n", cnf.zname);
		return 1;	
	}

	if (cnf.sw == SW_ALL) print_stats(&cnf);
	else if (cnf.sw == SW_READ_OPS)  print_stats_read_ops(&cnf);
	else if (cnf.sw == SW_WRITE_OPS) print_stats_write_ops(&cnf);
	else if (cnf.sw == SW_READ_BTS)	 print_stats_read_bts(&cnf);
	else if (cnf.sw == SW_WRITE_BTS) print_stats_write_bts(&cnf);
	else if (cnf.sw == SW_HEALTH) 	 print_stats_health_bool(&cnf);
	else if (cnf.sw == SW_LOGICAL)   print_stats_logical(&cnf);
	else if (cnf.sw == SW_COMPRESS)  print_stats_compress(&cnf);
	else if (cnf.sw == SW_USED) 	 print_stats_used(&cnf);
	else if (cnf.sw == SW_REAL_USED) print_stats_real_used(&cnf);
	else if (cnf.sw == SW_AVAILABLE) print_stats_available(&cnf);
	else if (cnf.sw == SW_DEDUPRATIO)print_stats_dedupratio(&cnf);



	free_devlist(&d);
	libzfs_fini(g_zfs);
	return 0;
}
