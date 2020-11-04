cmd_/home/chakim/Documents/exp/driver/modules.order := {   echo /home/chakim/Documents/exp/driver/hello.ko; :; } | awk '!x[$$0]++' - > /home/chakim/Documents/exp/driver/modules.order
