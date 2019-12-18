#!/bin/sh

VORTEX="192.168.1.232"
PORT=54000

# use POSOX locale for correct number conversions in awk
LC_NUMERIC=POSIX

# use shared memory tmpfs (super fast)
OUTFILE="/dev/shm/stats.out"
RESFILE="/dev/shm/response.out"

while true; do
    # clear any previous stats output file
    rm -f ${OUTFILE}

    # write each line to our stats output file
    # each output adds +0 to fields to convert them to numbers sans decorators
    echo +$(hostname)_mem "{\"time\":"$(date +%s)","$(free | awk '/^Mem/{print "\"mem_used\":" $3+0 "," "\"mem_total\":" $2+0}')"}" >> ${OUTFILE}
    echo +$(hostname)_swap "{\"time\":"$(date +%s)","$(free | awk '/^Swap/{print "\"swap_used\":" $3+0 "," "\"swap_total\":" $2+0}')"}" >> ${OUTFILE}
    #echo +$(hostname)_temp "{\"time\":"$(date +%s)","$(sensors | awk '/^temp1:/ { print "\"cpu_temp\":" $2+0.0}')"}" >> ${OUTFILE}
    echo +$(hostname)_disk0 "{\"time\":"$(date +%s)","$(df -H | awk '/root/ {print "\"disk0_used\":" $3+0 "," "\"disk0_avail\":" $4+0}')"}" >> ${OUTFILE}
    echo +$(hostname)_network "{\"time\":"$(date +%s)","$(cat /proc/net/dev | awk '/wlan0/ {print "\"net_recv\":" $2+0 "," "\"net_send\":" $9+0}')"}" >> ${OUTFILE}

    #output stats to vortex
    /opt/vortex/rcat -k1 -d50 ${VORTEX} ${PORT} ${OUTFILE} > ${RESFILE}

    sleep 60s

(true); done
