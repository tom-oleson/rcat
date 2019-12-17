#!/bin/sh

NAME=vortex_stats
DESC="Stats to VORTEX"
PIDFILE="/var/run/${NAME}.pid"
LOGFILE="/var/log/${NAME}.log"

# bin path
DAEMON="/bin/sh"

# Path the script
DAEMON_OPTS="/opt/vortex/stats.sh"

START_OPTS="--start --background --make-pidfile --pidfile ${PIDFILE} --exec ${DAEMON} ${DAEMON_OPTS}"
STOP_OPTS="--stop --pidfile ${PIDFILE}"

# does exec exit?
test -x $DAEMON || exit 0

set -e

case "$1" in
start)
echo -n "Starting ${DESC}: "
start-stop-daemon $START_OPTS >> $LOGFILE
echo "$NAME."
;;
stop)
echo -n "Stopping $DESC: "
start-stop-daemon $STOP_OPTS
echo "$NAME."
rm -f $PIDFILE
;;
restart|force-reload)
echo -n "Restarting $DESC: "
start-stop-daemon $STOP_OPTS
sleep 1
start-stop-daemon $START_OPTS >> $LOGFILE
echo "$NAME."
;;
*)
N=/etc/init.d/$NAME
echo "Usage: $N {start|stop|restart|force-reload}" >&2
exit 1
;;
esac

exit 0