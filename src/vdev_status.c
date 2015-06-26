//
// Created by burso on 6/18/15.
//

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "zio.h"
#include "config.h"
#include "vdev_status.h"
#include "memlist.h"

int
zpool_print_vdev(zpool_handle_t * zhp, void * data) {
    devlist_t * d = (devlist_t *)data;

    zpool_status_t reason;
    uint_t c;

    char * msgId = NULL;
    nvlist_t * config, * nvroot;
    vdev_stat_t * vs;

    config = zpool_get_config(zhp, NULL);
    reason = zpool_get_status(zhp, &msgId);

    verify(nvlist_lookup_nvlist(config, ZPOOL_CONFIG_VDEV_TREE, &nvroot) == 0);
    verify(nvlist_lookup_uint64_array(nvroot, ZPOOL_CONFIG_VDEV_STATS, (uint64_t **)&vs, &c) == 0);

    switch(reason) {
        case ZPOOL_STATUS_MISSING_DEV_R:
            (void) printf("status: One or more devices could not "
                                  "be opened.  Sufficient replicas exist for\n\tthe pool to "
                                  "continue functioning in a degraded state.\n");
            break;

        case ZPOOL_STATUS_MISSING_DEV_NR:
            (void) printf("status: One or more devices could not "
                                  "be opened.  There are insufficient\n\treplicas for the "
                                  "pool to continue functioning.\n");
            break;

        case ZPOOL_STATUS_CORRUPT_LABEL_R:
            (void) printf("status: One or more devices could not "
                                  "be used because the label is missing or\n\tinvalid.  "
                                  "Sufficient replicas exist for the pool to continue\n\t"
                                  "functioning in a degraded state.\n");
            (void) printf("action: Replace the device using "
                                  "'zpool replace'.\n");
            break;

        case ZPOOL_STATUS_CORRUPT_LABEL_NR:
            (void) printf("status: One or more devices could not "
                                  "be used because the label is missing \n\tor invalid.  "
                                  "There are insufficient replicas for the pool to "
                                  "continue\n\tfunctioning.\n");
            zpool_explain_recover(zpool_get_handle(zhp), zpool_get_name(zhp), reason, config);
            break;

        case ZPOOL_STATUS_FAILING_DEV:
            (void) printf("status: One or more devices has "
                                  "experienced an unrecoverable error.  An\n\tattempt was "
                                  "made to correct the error.  Applications are "
                                  "unaffected.\n");
            (void) printf("action: Determine if the device needs "
                                  "to be replaced, and clear the errors\n\tusing "
                                  "'zpool clear' or replace the device with 'zpool "
                                  "replace'.\n");
            break;

        case ZPOOL_STATUS_OFFLINE_DEV:
            (void) printf("status: One or more devices has "
                                  "been taken offline by the administrator.\n\tSufficient "
                                  "replicas exist for the pool to continue functioning in "
                                  "a\n\tdegraded state.\n");
            (void) printf("action: Online the device using "
                                  "'zpool online' or replace the device with\n\t'zpool "
                                  "replace'.\n");
            break;

        case ZPOOL_STATUS_REMOVED_DEV:
            (void) printf("status: One or more devices has "
                                  "been removed by the administrator.\n\tSufficient "
                                  "replicas exist for the pool to continue functioning in "
                                  "a\n\tdegraded state.\n");

            (void) printf("action: Online the device using "
                                  "'zpool online' or replace the device with\n\t'zpool "
                                  "replace'.\n");
            break;

        case ZPOOL_STATUS_RESILVERING:
            (void) printf("status: One or more devices is "
                                  "currently being resilvered.  The pool will\n\tcontinue "
                                  "to function, possibly in a degraded state.\n");
            (void) printf("action: Wait for the resilver to complete.\n");
            break;

        case ZPOOL_STATUS_CORRUPT_DATA:
            (void) printf("status: One or more devices has "
                                  "experienced an error resulting in data\n\tcorruption.  "
                                  "Applications may be affected.\n");
            (void) printf("action: Restore the file in question "
                                  "if possible.  Otherwise restore the\n\tentire pool from "
                                  "backup.\n");
            break;

        case ZPOOL_STATUS_CORRUPT_POOL:
            (void) printf("status: The pool metadata is corrupted "
                                  "and the pool cannot be opened.\n");
            zpool_explain_recover(zpool_get_handle(zhp), zpool_get_name(zhp), reason, config);
            break;

        case ZPOOL_STATUS_VERSION_OLDER:
            (void) printf("status: The pool is formatted using a "
                                  "legacy on-disk format.  The pool can\n\tstill be used, "
                                  "but some features are unavailable.\n");

            (void) printf("action: Upgrade the pool using 'zpool "
                                  "upgrade'.  Once this is done, the\n\tpool will no longer "
                                  "be accessible on software that does not support\n\t"
                                  "feature flags.\n");
            break;

        case ZPOOL_STATUS_VERSION_NEWER:
            (void) printf("status: The pool has been upgraded to a "
                                  "newer, incompatible on-disk version.\n\tThe pool cannot "
                                  "be accessed on this system.\n");
            (void) printf("action: Access the pool from a system "
                                  "running more recent software, or\n\trestore the pool from "
                                  "backup.\n");
            break;

        case ZPOOL_STATUS_FEAT_DISABLED:
            (void) printf("status: Some supported features are not "
                                  "enabled on the pool. The pool can\n\tstill be used, but "
                                  "some features are unavailable.\n");
            (void) printf("action: Enable all features using "
                                  "'zpool upgrade'. Once this is done,\n\tthe pool may no "
                                  "longer be accessible by software that does not support\n\t"
                                  "the features. See zpool-features(5) for details.\n");
            break;

        case ZPOOL_STATUS_UNSUP_FEAT_READ:
            (void) printf("status: The pool cannot be accessed on "
                                  "this system because it uses the\n\tfollowing feature(s) "
                                  "not supported on this system:\n");
            zpool_print_unsup_feat(config);
            (void) printf("\n");
            (void) printf("action: Access the pool from a system "
                                  "that supports the required feature(s),\n\tor restore the "
                                  "pool from backup.\n");
            break;

        case ZPOOL_STATUS_UNSUP_FEAT_WRITE:
            (void) printf("status: The pool can only be accessed "
                                  "in read-only mode on this system. It\n\tcannot be "
                                  "accessed in read-write mode because it uses the "
                                  "following\n\tfeature(s) not supported on this system:\n");
            zpool_print_unsup_feat(config);
            (void) printf("\n");
            (void) printf("action: The pool cannot be accessed in "
                                  "read-write mode. Import the pool with\n"
                                  "\t\"-o readonly=on\", access the pool from a system that "
                                  "supports the\n\trequired feature(s), or restore the "
                                  "pool from backup.\n");
            break;

        case ZPOOL_STATUS_FAULTED_DEV_R:
            (void) printf("status: One or more devices are "
                                  "faulted in response to persistent errors.\n\tSufficient "
                                  "replicas exist for the pool to continue functioning "
                                  "in a\n\tdegraded state.\n");
            (void) printf("action: Replace the faulted device, "
                                  "or use 'zpool clear' to mark the device\n\trepaired.\n");
            break;

        case ZPOOL_STATUS_FAULTED_DEV_NR:
            (void) printf("status: One or more devices are "
                                  "faulted in response to persistent errors.  There are "
                                  "insufficient replicas for the pool to\n\tcontinue "
                                  "functioning.\n");
            (void) printf("action: Destroy and re-create the pool "
                                  "from a backup source.  Manually marking the device\n"
                                  "\trepaired using 'zpool clear' may allow some data "
                                  "to be recovered.\n");
            break;

        case ZPOOL_STATUS_IO_FAILURE_WAIT:
        case ZPOOL_STATUS_IO_FAILURE_CONTINUE:
            (void) printf("status: One or more devices are "
                                  "faulted in response to IO failures.\n");
            (void) printf("action: Make sure the affected devices "
                                  "are connected, then run 'zpool clear'.\n");
            break;

        case ZPOOL_STATUS_BAD_LOG:
            (void) printf("status: An intent log record "
                                  "could not be read.\n"
                                  "\tWaiting for adminstrator intervention to fix the "
                                  "faulted pool.\n");
            (void) printf("action: Either restore the affected "
                                  "device(s) and run 'zpool online',\n"
                                  "\tor ignore the intent log records by running "
                                  "'zpool clear'.\n");

        case ZPOOL_STATUS_HOSTID_MISMATCH:
            (void) printf("status: Mismatch between pool hostid "
                                  "and system hostid on imported pool.\n\tThis pool was "
                                  "previously imported into a system with a different "
                                  "hostid,\n\tand then was verbatim imported into this "
                                  "system.\n");
            (void) printf("action: Export this pool on all systems "
                                  "on which it is imported.\n"
                                  "\tThen import it to correct the mismatch.\n");
            break;


        default:
            assert(reason == ZPOOL_STATUS_OK);
    }

    if (msgId != NULL)
        (void) printf("   see: http://zfsonlinux.org/msg/%s\n", msgId);

    if (config != NULL) {
        pool_scan_stat_t * ps = NULL;

        (void) nvlist_lookup_uint64_array(nvroot, ZPOOL_CONFIG_SCAN_STATS, (uint64_t **)&ps, &c);
        print_status_config(zhp, zpool_get_name(zhp), nvroot, d, 0, B_FALSE);

    }

    zpool_close(zhp);
    return 0;
}

void
print_status_config(zpool_handle_t * zhp, const char * name, nvlist_t * nv,
                    devlist_t * d, int depth, boolean_t isspare) {

    nvlist_t ** child;
    uint_t c, children;
    vdev_stat_t * vs;
    char * vname;
    const char * state = NULL;
    char * type = NULL;

    if (nvlist_lookup_nvlist_array(nv, ZPOOL_CONFIG_CHILDREN, &child, &children) != 0)
        children = 0;

    verify(nvlist_lookup_uint64_array(nv, ZPOOL_CONFIG_VDEV_STATS, (uint64_t **)&vs, &c) == 0);

    state = zpool_state_to_name(vs->vs_state, vs->vs_aux);

    for (c = 0; c < children; c++) {
        /*
        uint64_t islog = B_FALSE, ishole = B_FALSE;


        // Don't print logs or holes here
        (void) nvlist_lookup_uint64(child[c], ZPOOL_CONFIG_IS_LOG, &islog);
        (void) nvlist_lookup_uint64(child[c], ZPOOL_CONFIG_IS_HOLE, &ishole);
        if (ishole || islog)
            continue;

        */

        vname = zpool_vdev_name(g_zfs, zhp, child[c], B_TRUE);
        print_status_config(zhp, vname, child[c], d, depth + 2, isspare);
        free(vname);
    }


    verify(nvlist_lookup_string(nv, ZPOOL_CONFIG_TYPE, &type) == 0);

    if (strcmp(type, VDEV_TYPE_DISK) == 0) {
        add_to_devlist(d, name, state, zpool_get_name(zhp));
    }

    return;
}