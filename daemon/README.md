<pre>
         ______
        / ____ \
   ____/ /    \ \
  / ____/   x  \ \
 / /     __/   / / VORTEX_STATS
/ /  x__/  \  / /  Send Machine Stats to VORTEX service
\ \     \__/  \ \  Copyright (C) 2019, Tom Oleson, All Rights Reserved.
 \ \____   \   \ \ Made in the U.S.A.
  \____ \   x  / /
       \ \____/ /
        \______/
</pre>


# vortex_stats.sh
Runs stats.sh in a daemon service to send machine stats to vortex using rcat


To install, execute:
<pre>
sudo make -f linux.mk
</pre>

This will install vortex_stats.sh, stats.sh and a copy of rcat into /opt/vortex

NOTE: The make file assumes that rcat is up one directory and has already
been built using its own make file.


To start, stop, restart service, run in /opt/vortex

<pre>
sudo /opt/vortex/vortex_stats.sh start
sudo /opt/vortex/vortex_stats.sh stop
sudo /opt/vortex/vortex_stats.sh restart
</pre>

NOTE:
Configure the stats to send and the vortex instance to send to in the
stats.sh script.
