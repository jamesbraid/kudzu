"""generated from defs/xfs.toml by defs-gen - do not edit, change the source
XFS superblock and inode core as the shipped release writes them"""

UNIT = 'xfs'
REGISTERS = {}
WORDS = {'INO_W0': {'width': 32, 'grade': 'established'}, 'INO_W1': {'width': 32, 'grade': 'established'}, 'INO_W10': {'width': 32, 'grade': 'established'}, 'INO_W11': {'width': 32, 'grade': 'established'}, 'INO_W12': {'width': 32, 'grade': 'established'}, 'INO_W13': {'width': 32, 'grade': 'established'}, 'INO_W14': {'width': 32, 'grade': 'established'}, 'INO_W15': {'width': 32, 'grade': 'established'}, 'INO_W16': {'width': 32, 'grade': 'established'}, 'INO_W17': {'width': 32, 'grade': 'established'}, 'INO_W18': {'width': 32, 'grade': 'established'}, 'INO_W19': {'width': 32, 'grade': 'established'}, 'INO_W2': {'width': 32, 'grade': 'established'}, 'INO_W20': {'width': 32, 'grade': 'established'}, 'INO_W21': {'width': 32, 'grade': 'established'}, 'INO_W22': {'width': 32, 'grade': 'established'}, 'INO_W23': {'width': 32, 'grade': 'established'}, 'INO_W24': {'width': 32, 'grade': 'established'}, 'INO_W3': {'width': 32, 'grade': 'established'}, 'INO_W4': {'width': 32, 'grade': 'established'}, 'INO_W5': {'width': 32, 'grade': 'established'}, 'INO_W7': {'width': 32, 'grade': 'established'}, 'INO_W8': {'width': 32, 'grade': 'established'}, 'INO_W9': {'width': 32, 'grade': 'established'}, 'SB_W0': {'width': 32, 'grade': 'established'}, 'SB_W1': {'width': 32, 'grade': 'established'}, 'SB_W12': {'width': 32, 'grade': 'established'}, 'SB_W13': {'width': 32, 'grade': 'established'}, 'SB_W14': {'width': 32, 'grade': 'established'}, 'SB_W15': {'width': 32, 'grade': 'established'}, 'SB_W16': {'width': 32, 'grade': 'established'}, 'SB_W17': {'width': 32, 'grade': 'established'}, 'SB_W18': {'width': 32, 'grade': 'established'}, 'SB_W19': {'width': 32, 'grade': 'established'}, 'SB_W2': {'width': 32, 'grade': 'established'}, 'SB_W20': {'width': 32, 'grade': 'established'}, 'SB_W21': {'width': 32, 'grade': 'established'}, 'SB_W22': {'width': 32, 'grade': 'established'}, 'SB_W23': {'width': 32, 'grade': 'established'}, 'SB_W24': {'width': 32, 'grade': 'established'}, 'SB_W25': {'width': 32, 'grade': 'established'}, 'SB_W26': {'width': 32, 'grade': 'established'}, 'SB_W3': {'width': 32, 'grade': 'established'}, 'SB_W30': {'width': 32, 'grade': 'established'}, 'SB_W31': {'width': 32, 'grade': 'established'}, 'SB_W32': {'width': 32, 'grade': 'established'}, 'SB_W33': {'width': 32, 'grade': 'established'}, 'SB_W34': {'width': 32, 'grade': 'established'}, 'SB_W35': {'width': 32, 'grade': 'established'}, 'SB_W36': {'width': 32, 'grade': 'established'}, 'SB_W37': {'width': 32, 'grade': 'established'}, 'SB_W38': {'width': 32, 'grade': 'established'}, 'SB_W39': {'width': 32, 'grade': 'established'}, 'SB_W4': {'width': 32, 'grade': 'established'}, 'SB_W40': {'width': 32, 'grade': 'established'}, 'SB_W41': {'width': 32, 'grade': 'established'}, 'SB_W42': {'width': 32, 'grade': 'established'}, 'SB_W43': {'width': 32, 'grade': 'established'}, 'SB_W44': {'width': 32, 'grade': 'established'}, 'SB_W45': {'width': 32, 'grade': 'established'}, 'SB_W46': {'width': 32, 'grade': 'established'}, 'SB_W47': {'width': 32, 'grade': 'established'}, 'SB_W48': {'width': 32, 'grade': 'established'}, 'SB_W49': {'width': 32, 'grade': 'established'}, 'SB_W5': {'width': 32, 'grade': 'established'}, 'SB_W50': {'width': 32, 'grade': 'established'}, 'SB_W6': {'width': 32, 'grade': 'established'}, 'SB_W7': {'width': 32, 'grade': 'established'}}
FIELDS = {'INO_W0': {'magic': (31, 16), 'mode': (15, 0)}, 'INO_W1': {'version': (31, 24), 'format': (23, 16), 'onlink': (15, 0)}, 'INO_W10': {'mtime_sec': (31, 0)}, 'INO_W11': {'mtime_nsec': (31, 0)}, 'INO_W12': {'ctime_sec': (31, 0)}, 'INO_W13': {'ctime_nsec': (31, 0)}, 'INO_W14': {'size_hi': (31, 0)}, 'INO_W15': {'size_lo': (31, 0)}, 'INO_W16': {'nblocks_hi': (31, 0)}, 'INO_W17': {'nblocks_lo': (31, 0)}, 'INO_W18': {'extsize': (31, 0)}, 'INO_W19': {'nextents': (31, 0)}, 'INO_W2': {'uid': (31, 0)}, 'INO_W20': {'naextents': (31, 16), 'forkoff': (15, 8), 'aformat': (7, 0)}, 'INO_W21': {'dmevmask': (31, 0)}, 'INO_W22': {'dmstate': (31, 16), 'flags_rtinherit': (8, 8), 'flags_nodump': (7, 7), 'flags_noatime': (6, 6), 'flags_sync': (5, 5), 'flags_append': (4, 4), 'flags_immutable': (3, 3), 'flags_newrtbm': (2, 2), 'flags_prealloc': (1, 1), 'flags_realtime': (0, 0)}, 'INO_W23': {'gen': (31, 0)}, 'INO_W24': {'next_unlinked': (31, 0)}, 'INO_W3': {'gid': (31, 0)}, 'INO_W4': {'nlinkv2': (31, 0)}, 'INO_W5': {'projid': (31, 16)}, 'INO_W7': {'flushiter': (15, 0)}, 'INO_W8': {'atime_sec': (31, 0)}, 'INO_W9': {'atime_nsec': (31, 0)}, 'SB_W0': {'magicnum': (31, 0)}, 'SB_W1': {'blocksize': (31, 0)}, 'SB_W12': {'logstart_hi': (31, 0)}, 'SB_W13': {'logstart_lo': (31, 0)}, 'SB_W14': {'rootino_hi': (31, 0)}, 'SB_W15': {'rootino_lo': (31, 0)}, 'SB_W16': {'rbmino_hi': (31, 0)}, 'SB_W17': {'rbmino_lo': (31, 0)}, 'SB_W18': {'rsumino_hi': (31, 0)}, 'SB_W19': {'rsumino_lo': (31, 0)}, 'SB_W2': {'dblocks_hi': (31, 0)}, 'SB_W20': {'rextsize': (31, 0)}, 'SB_W21': {'agblocks': (31, 0)}, 'SB_W22': {'agcount': (31, 0)}, 'SB_W23': {'rbmblocks': (31, 0)}, 'SB_W24': {'logblocks': (31, 0)}, 'SB_W25': {'versionnum_morebits': (31, 31), 'versionnum_ci_dirs': (30, 30), 'versionnum_dirv2': (29, 29), 'versionnum_extflg': (28, 28), 'versionnum_rejected': (27, 27), 'versionnum_logv2': (26, 26), 'versionnum_shared': (25, 25), 'versionnum_dalign': (24, 24), 'versionnum_align': (23, 23), 'versionnum_quota': (22, 22), 'versionnum_nlink': (21, 21), 'versionnum_attr': (20, 20), 'versionnum_vers': (19, 16), 'sectsize': (15, 0)}, 'SB_W26': {'inodesize': (31, 16), 'inopblock': (15, 0)}, 'SB_W3': {'dblocks_lo': (31, 0)}, 'SB_W30': {'blocklog': (31, 24), 'sectlog': (23, 16), 'inodelog': (15, 8), 'inopblog': (7, 0)}, 'SB_W31': {'agblklog': (31, 24), 'rextslog': (23, 16), 'inprogress': (15, 8), 'imax_pct': (7, 0)}, 'SB_W32': {'icount_hi': (31, 0)}, 'SB_W33': {'icount_lo': (31, 0)}, 'SB_W34': {'ifree_hi': (31, 0)}, 'SB_W35': {'ifree_lo': (31, 0)}, 'SB_W36': {'fdblocks_hi': (31, 0)}, 'SB_W37': {'fdblocks_lo': (31, 0)}, 'SB_W38': {'frextents_hi': (31, 0)}, 'SB_W39': {'frextents_lo': (31, 0)}, 'SB_W4': {'rblocks_hi': (31, 0)}, 'SB_W40': {'uquotino_hi': (31, 0)}, 'SB_W41': {'uquotino_lo': (31, 0)}, 'SB_W42': {'pquotino_hi': (31, 0)}, 'SB_W43': {'pquotino_lo': (31, 0)}, 'SB_W44': {'qflags': (31, 16), 'flags_shared_ro': (8, 8), 'shared_vn': (7, 0)}, 'SB_W45': {'inoalignmt': (31, 0)}, 'SB_W46': {'unit': (31, 0)}, 'SB_W47': {'width': (31, 0)}, 'SB_W48': {'dirblklog': (31, 24), 'logsectlog': (23, 16), 'logsectsize': (15, 0)}, 'SB_W49': {'logsunit': (31, 0)}, 'SB_W5': {'rblocks_lo': (31, 0)}, 'SB_W50': {'features2_high': (31, 5), 'features2_parent': (4, 4), 'features2_attr2': (3, 3), 'features2_bit2': (2, 2), 'features2_lazysbcount': (1, 1), 'features2_bit0': (0, 0)}, 'SB_W6': {'rextents_hi': (31, 0)}, 'SB_W7': {'rextents_lo': (31, 0)}}
ENUMS = {'INO_W1.format': {0: 'dev', 1: 'local', 2: 'extents', 3: 'btree', 4: 'uuid'}, 'INO_W20.aformat': {1: 'local', 2: 'extents', 3: 'btree'}}
FIELD_ENUM = {'INO_W1.format': 'INO_W1.format', 'INO_W20.aformat': 'INO_W20.aformat'}
CONSTANTS = {'INO_ATTR_FORK_UNIT': {'value': 8, 'count': 1, 'stride': 1}, 'INO_FLAGS_OFFSET': {'value': 90, 'count': 1, 'stride': 1}, 'INO_FORK_OFFSET': {'value': 100, 'count': 1, 'stride': 1}, 'INO_MAGIC': {'value': 18766, 'count': 1, 'stride': 1}, 'INO_SIZE': {'value': 100, 'count': 1, 'stride': 1}, 'SB_DESCRIBED_BYTES': {'value': 204, 'count': 1, 'stride': 1}, 'SB_LEN_FNAME': {'value': 6, 'count': 1, 'stride': 1}, 'SB_LEN_FPACK': {'value': 6, 'count': 1, 'stride': 1}, 'SB_LEN_UUID': {'value': 16, 'count': 1, 'stride': 1}, 'SB_MAGIC': {'value': 1481003842, 'count': 1, 'stride': 1}, 'SB_OFF_FNAME': {'value': 108, 'count': 1, 'stride': 1}, 'SB_OFF_FPACK': {'value': 114, 'count': 1, 'stride': 1}, 'SB_OFF_UUID': {'value': 32, 'count': 1, 'stride': 1}, 'SB_READ_BYTES': {'value': 208, 'count': 1, 'stride': 1}, 'SB_SIZE': {'value': 208, 'count': 1, 'stride': 1}}
TESTS = [('SB_W50', 27, {'features2_bit0': 1, 'features2_lazysbcount': 1, 'features2_attr2': 1, 'features2_parent': 1})]


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
