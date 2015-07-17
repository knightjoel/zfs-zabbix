# zfs-zabbix
zabbix advanced monitoring of zfs

###### help

<pre>
zio --help
zio: [-s <show>][-f <format>][-z <zpool>][-d <device>]

 -z | --zpool           set zpool
 -s | --show            show zpools name
 -d | --device          set device
 -f | --format          set type of format [text, json]
 -h | --help            show this help menu
 -v | --version         show version

parameters for -s:

        all             - print all statistics
        read_ops        - print read io operations
        write_ops       - print write io operations
        read_bts        - print read bytes per seconds
        write_bts       - print write bytes per seconds
        health          - print health of zpool
        logical         - print logical used space
        compress        - print space saved by compress
        dedupratio      - print dedupratio of zpool
        used            - print used space
        real            - print real used space after dedup
        available       - print available space
        pools           - print pools
        devices         - print devices in zpool
        device-state    - print state of device
        ddt-memory      - print size of deduplication table in memory
</pre>
