# tg: match table of the gigabit Ethernet driver

A software table, not a register map: the PCI vendor, device and subsystem ids that the Origin 300/350 gigabit Ethernet driver (`tg`) accepts. [format.toml](format.toml) holds the values with their grades and [format.md](format.md) is the generated table.

What the driver expects of the controller at attach and open is read in [ip35-tg-driver.md](../../software/kernel/ip35-tg-driver.md).

## Used by

- [IP35: what the gigabit Ethernet driver (`tg`) expects of its controller at attach and open](../../software/kernel/ip35-tg-driver.md)
