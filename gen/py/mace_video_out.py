"""generated from defs/mace_video_out.toml by defs-gen - do not edit, change the source
O2 MACE video output channel registers as the IRIX O2 video driver reaches them"""

UNIT = 'mace_video_out'
REGISTERS = {'out_control': {'offset': 0, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'out_status': {'offset': 8, 'access': 'r', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'out_config': {'offset': 16, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'out_next_descriptor': {'offset': 24, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'out_field_offset': {'offset': 32, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'out_field_size': {'offset': 40, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}, 'out_h_pad_odd': {'offset': 48, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'out_v_pad_odd': {'offset': 56, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'out_h_pad_even': {'offset': 64, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'out_v_pad_even': {'offset': 72, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'out_genlock_delay': {'offset': 80, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'out_hw_config': {'offset': 88, 'access': 'rw', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'inferred'}}
WORDS = {}
FIELDS = {'out_control': {'intr_enable': (8, 1), 'dma_enable': (0, 0)}, 'out_status': {}, 'out_config': {}, 'out_next_descriptor': {}, 'out_field_offset': {'offset': (15, 0)}, 'out_field_size': {'lines': (21, 12), 'line_bytes': (11, 0)}, 'out_h_pad_odd': {}, 'out_v_pad_odd': {}, 'out_h_pad_even': {}, 'out_v_pad_even': {}, 'out_genlock_delay': {}, 'out_hw_config': {'ident': (35, 32), 'sof_count_even': (22, 19), 'general_output': (18, 18), 'sof_count_odd': (17, 14), 'framelock_correction': (13, 13), 'framelock_precision': (12, 12), 'framelock_source': (11, 10), 'framelock_reset': (9, 9), 'audio_sync_source': (6, 5), 'port_cd_select': (3, 3), 'port_cd_reset': (2, 2), 'port_ab_select': (1, 1), 'port_ab_reset': (0, 0)}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {}
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
