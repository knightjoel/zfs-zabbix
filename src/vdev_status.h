//
// Created by burso on 6/18/15.
//

#ifndef ZFS_ZABBIX_VDEV_STATUS_H
#define ZFS_ZABBIX_VDEV_STATUS_H
#include <sys/fs/zfs.h>
#include <libzfs.h>
#include "memlist.h"


int zpool_print_vdev(zpool_handle_t * zhp, void * data);
void print_spares(zpool_handle_t * zhp, nvlist_t ** spares, uint_t nspares, devlist_t * d);
void print_status_config(zpool_handle_t * zhp, const char * name, nvlist_t * nv,
                         devlist_t * d, int depth, boolean_t isspare);



#endif //ZFS_ZABBIX_VDEV_STATUS_H
