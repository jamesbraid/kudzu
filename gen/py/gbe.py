"""generated from defs/gbe.toml by defs-gen - do not edit, change the source
O2 GBE register block as the O2 kernel drives it: two-wire buses, flat-panel timing words and the dual-channel option probe"""

UNIT = 'gbe'
REGISTERS = {'vt_vblank': {'offset': 65552, 'access': 'w', 'width': 32, 'count': 1, 'stride': 4, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'vt_hblank': {'offset': 65556, 'access': 'w', 'width': 32, 'count': 1, 'stride': 4, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'timing_flags': {'offset': 65560, 'access': 'rw', 'width': 32, 'count': 1, 'stride': 4, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'fp_hdrv': {'offset': 65576, 'access': 'w', 'width': 32, 'count': 1, 'stride': 4, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'fp_vdrv': {'offset': 65580, 'access': 'w', 'width': 32, 'count': 1, 'stride': 4, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'fp_de': {'offset': 65584, 'access': 'w', 'width': 32, 'count': 1, 'stride': 4, 'space': None, 'reset': 'unknown', 'grade': 'established'}}
WORDS = {}
FIELDS = {'vt_vblank': {'hi': (23, 12), 'lo': (11, 0)}, 'vt_hblank': {'hi': (23, 12), 'lo': (11, 0)}, 'timing_flags': {}, 'fp_hdrv': {'hi': (23, 12), 'lo': (11, 0)}, 'fp_vdrv': {'hi': (23, 12), 'lo': (11, 0)}, 'fp_de': {'hi': (23, 12), 'lo': (11, 0)}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'BOARD_INTERLACE_OFF': {'value': 45, 'count': 1, 'stride': 1}, 'BOARD_PANEL_OFF': {'value': 46, 'count': 1, 'stride': 1}, 'FIELD_MASK': {'value': 4095, 'count': 1, 'stride': 1}, 'FIELD_SHIFT': {'value': 12, 'count': 1, 'stride': 1}, 'OPT_CMD_CLOCK_KHZ': {'value': 100000, 'count': 1, 'stride': 1}, 'OPT_CMD_ID_HIGH': {'value': 255, 'count': 1, 'stride': 1}, 'OPT_CMD_ID_LOW': {'value': 127, 'count': 1, 'stride': 1}, 'OPT_DELAY_SMALL': {'value': 2, 'count': 1, 'stride': 1}, 'OPT_ID_MASK': {'value': 96, 'count': 1, 'stride': 1}, 'OPT_ID_RESTORE': {'value': 112, 'count': 1, 'stride': 1}, 'OPT_INSTALLED_LIMIT': {'value': 9, 'count': 1, 'stride': 1}, 'OPT_PORT_ADDR': {'value': 126, 'count': 1, 'stride': 1}, 'PANEL_SETUP_A': {'value': 130, 'count': 1, 'stride': 1}, 'PANEL_SETUP_B': {'value': 131, 'count': 1, 'stride': 1}, 'PANEL_SETUP_C': {'value': 132, 'count': 1, 'stride': 1}, 'TWO_WIRE_FP_OFF': {'value': 16, 'count': 1, 'stride': 1}, 'TWO_WIRE_MON_OFF': {'value': 8, 'count': 1, 'stride': 1}, 'TWO_WIRE_WAIT_BIT': {'value': 1, 'count': 1, 'stride': 1}, 'TWO_WIRE_WAIT_LIMIT': {'value': 60, 'count': 1, 'stride': 1}}
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
