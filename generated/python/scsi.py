"""generated from formats/scsi/format.toml by defs-gen - do not edit, change the source
SCSI command opcodes the CD-ROM driver selects by drive class"""

UNIT = 'scsi'
REGISTERS = {}
WORDS = {}
FIELDS = {}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'CDROM_VENDOR_C4': {'value': 196, 'count': 1, 'stride': 1}, 'CDROM_VENDOR_C9': {'value': 201, 'count': 1, 'stride': 1}, 'INQ_LEN_PRODUCT': {'value': 16, 'count': 1, 'stride': 1}, 'INQ_LEN_VENDOR': {'value': 8, 'count': 1, 'stride': 1}, 'INQ_OFF_PRODUCT': {'value': 16, 'count': 1, 'stride': 1}, 'INQ_OFF_TYPE': {'value': 0, 'count': 1, 'stride': 1}, 'INQ_OFF_VENDOR': {'value': 8, 'count': 1, 'stride': 1}, 'INQ_OFF_VERSION': {'value': 2, 'count': 1, 'stride': 1}, 'INQ_TYPE_CDROM': {'value': 5, 'count': 1, 'stride': 1}, 'INQ_TYPE_DISK': {'value': 0, 'count': 1, 'stride': 1}, 'INQ_TYPE_TAPE': {'value': 1, 'count': 1, 'stride': 1}, 'INQ_VERSION_MASK': {'value': 7, 'count': 1, 'stride': 1}, 'INQ_VERSION_REFINE': {'value': 2, 'count': 1, 'stride': 1}, 'PROM_PREFIX_LEN': {'value': 10, 'count': 1, 'stride': 1}, 'START_STOP_UNIT': {'value': 27, 'count': 1, 'stride': 1}}
TESTS = []


def decode(name, value):
    """Field name -> value for a register or word."""
    return {f: (value >> lo) & ((1 << (hi - lo + 1)) - 1) for f, (hi, lo) in FIELDS[name].items()}


def encode(name, **fields):
    v = 0
    for f, x in fields.items():
        hi, lo = FIELDS[name][f]
        if x >> (hi - lo + 1):
            raise ValueError('%s.%s: %d does not fit' % (name, f, x))
        v |= x << lo
    return v


def describe(name, value):
    """decode, with enum labels where the field has an enum."""
    out = {}
    for f, x in decode(name, value).items():
        e = ENUMS.get(FIELD_ENUM.get('%s.%s' % (name, f)))
        out[f] = e.get(x, x) if e else x
    return out


def selftest():
    for name, value, expect in TESTS:
        got = decode(name, value)
        for f, want in expect.items():
            assert got[f] == want, (name, hex(value), f, got[f], want)
    return len(TESTS)


if __name__ == '__main__':
    print('%s: %d vectors ok' % (UNIT, selftest()))
