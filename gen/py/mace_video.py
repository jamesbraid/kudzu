"""generated from defs/mace_video.toml by defs-gen - do not edit, change the source
O2 MACE video window map and input channel registers as the IRIX O2 video driver reaches them"""

UNIT = 'mace_video'
REGISTERS = {'in_control': {'offset': 0, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'in_status': {'offset': 8, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'in_config': {'offset': 16, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'in_next_descriptor': {'offset': 24, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'in_field_offset': {'offset': 32, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'in_line_width': {'offset': 40, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'in_hclip_odd': {'offset': 48, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'in_vclip_odd': {'offset': 56, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'in_alpha_odd': {'offset': 64, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'in_hclip_even': {'offset': 72, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'in_vclip_even': {'offset': 80, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'in_alpha_even': {'offset': 88, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}}
WORDS = {}
FIELDS = {'in_control': {'intr_enable': (8, 1), 'dma_enable': (0, 0)}, 'in_status': {}, 'in_config': {}, 'in_next_descriptor': {}, 'in_field_offset': {'offset': (15, 0)}, 'in_line_width': {}, 'in_hclip_odd': {}, 'in_vclip_odd': {}, 'in_alpha_odd': {}, 'in_hclip_even': {}, 'in_vclip_even': {}, 'in_alpha_even': {}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'GBE_CAPTURE_OFF': {'value': 524288, 'count': 1, 'stride': 1}, 'IN2_OFF': {'value': 524288, 'count': 1, 'stride': 1}, 'OUT_OFF': {'value': 1048576, 'count': 1, 'stride': 1}, 'TIMER_PHYS_ADDR': {'value': 523501568, 'count': 1, 'stride': 1}, 'TIMER_SNAP_IN1_OFF': {'value': 56, 'count': 1, 'stride': 1}, 'TIMER_SNAP_IN2_OFF': {'value': 64, 'count': 1, 'stride': 1}, 'TIMER_SNAP_OUT_OFF': {'value': 72, 'count': 1, 'stride': 1}, 'TWO_WIRE_PHYS_ADDR': {'value': 523436032, 'count': 1, 'stride': 1}, 'WINDOW_PHYS_ADDR': {'value': 521142272, 'count': 1, 'stride': 1}}
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
