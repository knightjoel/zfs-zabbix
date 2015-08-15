# zfs-zabbix
zabbix advanced monitoring of zfs

#### help

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

#### installation

##### omnios:
<pre>
~# gmake CC="path_to_compiler -DSOLARIS"
</pre>

##### openindiana:<br>
<pre>
~# gmake CC="path_to_compiler -DOI"
</pre>

hint: problem with linker
<pre>
ld: fatal: file crt1.o: open failed: No such file or directory
collect2: ld returned 1 exit status
gmake: *** [zio] Error 1
</pre>

solution: install package "lint"
<pre>
~# pkg install pkg://openindiana.org/developer/library/lint
</pre>



##### freebsd:
<pre>
~# gmake -f Makefile
</pre>

##### linux:
<pre>
~# make -f Makefile
</pre>
