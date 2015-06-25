#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/fs/zfs.h>
#include <sys/types.h>
#include <libzfs.h>
#include <string.h>

#include "fsutil.h"

int
zpool_get_count(__attribute__((unused)) zpool_handle_t * zhp, void * data) {
        unsigned int * iter = (unsigned int *)data;
        (*iter)++;
        return 0;
}

int
show_zpools_name_json(zpool_handle_t * zhp, void * data) {
        static unsigned int iter = 0;
        unsigned int * zpool_count = (unsigned int *)data;

        fprintf(stdout, "\t\t{ ");
        fprintf(stdout, "\"{#FSNAME}\":\"%s\",", zpool_get_name(zhp));
        fprintf(stdout, "\t\t\"{#FSTYPE}\":\"zfs\"");

        iter++;
        if (iter != *zpool_count) fprintf(stdout, " },\n");
        else fprintf(stdout, " }\n");

        zpool_close(zhp);
        return 0;
}

int
show_zpools_json(libzfs_handle_t * g_zfs) {
        unsigned int zpool_count = 0;
        zpool_iter(g_zfs, zpool_get_count, (void *)&zpool_count);

        fprintf(stdout, "{\n\t\"data\":[\n\n");
        zpool_iter(g_zfs, show_zpools_name_json, (void *)&zpool_count);
        fprintf(stdout, "\n\t]\n");
        fprintf(stdout, "}\n");

        return 0;
}

int 
show_zpools_name(zpool_handle_t * zhp, void * data __attribute__((unused))) {
	fprintf(stdout, "%s\n", zpool_get_name(zhp));
	zpool_close(zhp);
	return 0;
}

int 
show_zpools(libzfs_handle_t * g_zfs) {
	return (zpool_iter(g_zfs, show_zpools_name, NULL));
}

float
zpool_get_dedupratio(zpool_handle_t * zhp) {
	char dedupratio_str[32];
	float dedupratio = 0;

	zpool_get_prop(zhp, ZPOOL_PROP_DEDUPRATIO, dedupratio_str, sizeof(dedupratio_str), NULL, false);
	dedupratio = atof(dedupratio_str);

	return dedupratio;
}

const char *
zpool_get_health(zpool_handle_t * zhp) {
	static char health[32];

	zpool_get_prop(zhp, ZPOOL_PROP_HEALTH, health, sizeof(health), NULL, false);
	return health;
}

const char *
zpool_get_poolname(zpool_handle_t * zhp) {
	static char zpool_name[ZAP_MAXNAMELEN];	


	zpool_get_prop(zhp, ZPOOL_PROP_NAME, zpool_name, sizeof(zpool_name), NULL, false);
	return zpool_name;
}

float
zfs_get_compressratio(zfs_handle_t * zhf) {
	char compressratio_str[32];
	float compressratio = 0;

	zfs_prop_get(zhf, ZFS_PROP_COMPRESSRATIO, compressratio_str, sizeof(compressratio_str), NULL, NULL, 0, false);
	compressratio = atof(compressratio_str);		

	return compressratio;
}

long long unsigned int
zfs_get_used(zfs_handle_t * zhf) {
	long long unsigned int zfs_used = 0;
	char zfs_used_str[256];
	
	zfs_prop_get(zhf, ZFS_PROP_USED, zfs_used_str, sizeof(zfs_used_str), NULL, NULL, 0, true);
	zfs_used = atoll(zfs_used_str);

	return zfs_used;
}

long long unsigned int
zfs_get_available(zfs_handle_t * zhf) {
	long long unsigned int zfs_available = 0;
	char zfs_available_str[256];

	zfs_prop_get(zhf, ZFS_PROP_AVAILABLE, zfs_available_str, sizeof(zfs_available_str), NULL, NULL, 0, true);
	zfs_available = atoll(zfs_available_str);
	
	return zfs_available;
}

long long unsigned int
zfs_get_logical(zfs_handle_t * zhf) {
	long long unsigned int zfs_logical = 0;
	char zfs_logical_str[256];

	zfs_prop_get(zhf, ZFS_PROP_LOGICALUSED, zfs_logical_str, sizeof(zfs_logical_str), NULL, NULL, 0, true);
	zfs_logical = atoll(zfs_logical_str);	

	return zfs_logical;
}

const char * 
zfs_get_fsname(zfs_handle_t * zhf) {
	static char zfs_name[ZAP_MAXNAMELEN];
	
	zfs_prop_get(zhf, ZFS_PROP_NAME, zfs_name, sizeof(zfs_name), NULL, NULL, 0, true);	
	return zfs_name;
}
