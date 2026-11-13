# crime_dma: the pixel-DMA request

A software table, not a register map: the size of the request the IRIX kernel builds for the O2 renderer's pixel DMA, how many times it polls for idle and the ioctl request number that carries it. [format.toml](format.toml) holds the values with their grades and [format.md](format.md) is the generated table.

The CRIME registers the request drives are described in the crime repository. How the shipped graphics code uses them is read in [crime.md](../../software/video/crime.md).

## Used by

- [O2 CRIME: identity, interrupts, renderer interface and pixel pipeline](../../software/video/crime.md)
