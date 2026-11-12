"""generated from defs/crime.toml by defs-gen - do not edit, change the source
O2 CRIME as the shipped IRIX kernel and graphics libraries drive it: identity, interrupt inputs, renderer state and TLB formats"""

UNIT = 'crime'
REGISTERS = {'id': {'offset': 0, 'access': 'r', 'width': 8, 'count': 1, 'stride': 1, 'space': None, 'reset': 'unknown', 'grade': 'established'}, 'texture_format': {'offset': 8472, 'access': 'w', 'width': 64, 'count': 1, 'stride': 8, 'space': None, 'reset': 'unknown', 'grade': 'established'}}
WORDS = {'IB_ADDR': {'width': 64, 'grade': 'established'}}
FIELDS = {'id': {'family': (7, 4)}, 'texture_format': {'width_inv': (47, 44), 'height_inv': (43, 40), 'width_log2': (39, 36), 'height_log2': (35, 32), 'width_mask': (31, 16), 'height_mask': (15, 0)}, 'IB_ADDR': {'stop': (45, 45), 'lane': (44, 43), 'group': (42, 40), 'reg_index': (39, 32)}}
ENUMS = {}
FIELD_ENUM = {}
CONSTANTS = {'AA_LINE_OFF': {'value': 8592, 'count': 1, 'stride': 1}, 'ALPHA_TEST_OFF': {'value': 8600, 'count': 1, 'stride': 1}, 'BLEND_FUNC_OFF': {'value': 8616, 'count': 1, 'stride': 1}, 'BLOCK_ADDR': {'value': 335544320, 'count': 1, 'stride': 1}, 'CID_TLB_REGS': {'value': 4, 'count': 1, 'stride': 1}, 'CONTROL_OFF': {'value': 8, 'count': 1, 'stride': 1}, 'CPU_ERR_STATUS_OFF': {'value': 72, 'count': 1, 'stride': 1}, 'DEPTH_APERTURE_ADDR': {'value': 301989888, 'count': 1, 'stride': 1}, 'DEPTH_FUNC_OFF': {'value': 8640, 'count': 1, 'stride': 1}, 'DMA_IOCTL': {'value': 22002, 'count': 1, 'stride': 1}, 'DMA_POLL_LIMIT': {'value': 500000, 'count': 1, 'stride': 1}, 'DMA_REQ_BYTES': {'value': 40, 'count': 1, 'stride': 1}, 'DRAWMODE_OFF': {'value': 8216, 'count': 1, 'stride': 1}, 'ENGINE_RESET_OFF': {'value': 1032, 'count': 1, 'stride': 1}, 'FB_TLB_REGS': {'value': 64, 'count': 1, 'stride': 1}, 'FOG_COLOR_OFF': {'value': 8560, 'count': 1, 'stride': 1}, 'FRAME_APERTURE_ADDR': {'value': 268435456, 'count': 1, 'stride': 1}, 'FULL_MEM_ADDR': {'value': 1073741824, 'count': 1, 'stride': 1}, 'GBE_WINDOW_ADDR': {'value': 369098752, 'count': 1, 'stride': 1}, 'GL_UNBOUND_BIAS': {'value': 135168, 'count': 1, 'stride': 1}, 'GL_VERTEX_BIAS': {'value': 4096, 'count': 1, 'stride': 1}, 'HIGH_ALIAS_ADDR': {'value': 536870912, 'count': 1, 'stride': 1}, 'ID_FAMILY_ACCEPTED': {'value': 10, 'count': 1, 'stride': 1}, 'ID_REFUSED_BYTE': {'value': 160, 'count': 1, 'stride': 1}, 'ID_SPEC_15': {'value': 177, 'count': 1, 'stride': 1}, 'INTMASK_OFF': {'value': 24, 'count': 1, 'stride': 1}, 'INTR_EDGE_OFF': {'value': 40, 'count': 1, 'stride': 1}, 'INTSTAT_OFF': {'value': 16, 'count': 1, 'stride': 1}, 'IRQ_BUS_ERR_A': {'value': 20, 'count': 1, 'stride': 1}, 'IRQ_BUS_ERR_B': {'value': 21, 'count': 1, 'stride': 1}, 'IRQ_FIFO_HIGH': {'value': 26, 'count': 1, 'stride': 1}, 'IRQ_FIFO_LOW': {'value': 25, 'count': 1, 'stride': 1}, 'IRQ_GBE_LINE_2': {'value': 18, 'count': 1, 'stride': 1}, 'IRQ_GBE_LINE_3': {'value': 19, 'count': 1, 'stride': 1}, 'IRQ_MACE_ISA_A': {'value': 4, 'count': 1, 'stride': 1}, 'IRQ_MACE_ISA_B': {'value': 5, 'count': 1, 'stride': 1}, 'IRQ_PREBLANK': {'value': 17, 'count': 1, 'stride': 1}, 'IRQ_RETRACE': {'value': 16, 'count': 1, 'stride': 1}, 'IRQ_SOFT_A': {'value': 28, 'count': 1, 'stride': 1}, 'IRQ_SOFT_B': {'value': 29, 'count': 1, 'stride': 1}, 'LINEAR_WINDOW_MASK': {'value': 131071, 'count': 1, 'stride': 1}, 'LINE_MATCH_LATCHES': {'value': 4, 'count': 1, 'stride': 1}, 'LIN_TLB_MAX_PAGES': {'value': 32, 'count': 1, 'stride': 1}, 'LIN_TLB_REGS': {'value': 16, 'count': 1, 'stride': 1}, 'LIN_TLB_VALID': {'value': 2147483648, 'count': 1, 'stride': 1}, 'LOGIC_OP_OFF': {'value': 8624, 'count': 1, 'stride': 1}, 'MACE_WINDOW_ADDR': {'value': 402653184, 'count': 1, 'stride': 1}, 'MEM_CTL_OFF': {'value': 512, 'count': 1, 'stride': 1}, 'MEM_ERR_STATUS_OFF': {'value': 592, 'count': 1, 'stride': 1}, 'MTE_HOST_CHUNK_BYTES': {'value': 131072, 'count': 1, 'stride': 1}, 'MTE_MODE_HOST_CLEAR': {'value': 17, 'count': 1, 'stride': 1}, 'MTE_MODE_HOST_COPY': {'value': 2199, 'count': 1, 'stride': 1}, 'MTE_MODE_X_COPY': {'value': 2080, 'count': 1, 'stride': 1}, 'MTE_X_BLOCK_BYTES': {'value': 256, 'count': 1, 'stride': 1}, 'MTE_X_ROWS_16': {'value': 16, 'count': 1, 'stride': 1}, 'MTE_X_ROWS_32': {'value': 8, 'count': 1, 'stride': 1}, 'NOECC_ALIAS_ADDR': {'value': 2147483648, 'count': 1, 'stride': 1}, 'PAGE_BYTES': {'value': 4096, 'count': 1, 'stride': 1}, 'PIXFLUSH_OFF': {'value': 8696, 'count': 1, 'stride': 1}, 'PIXPIPE_NULL_OFF': {'value': 8688, 'count': 1, 'stride': 1}, 'PIXXFER_POS_OFF': {'value': 8352, 'count': 1, 'stride': 1}, 'PIXXFER_VTX_OFF': {'value': 8304, 'count': 1, 'stride': 1}, 'PIXXFER_YSTEP_OFF': {'value': 8364, 'count': 1, 'stride': 1}, 'PRIMITIVE_OFF': {'value': 8288, 'count': 1, 'stride': 1}, 'PRIM_RECT': {'value': 50462720, 'count': 1, 'stride': 1}, 'PRIM_TRIANGLE': {'value': 33554432, 'count': 1, 'stride': 1}, 'RENDER_COUNT_INIT': {'value': 10, 'count': 1, 'stride': 1}, 'RENDER_CTL_INIT': {'value': 267386634, 'count': 1, 'stride': 1}, 'RENDER_CTL_OFF': {'value': 1024, 'count': 1, 'stride': 1}, 'RENDER_EMPTY_SHIFT': {'value': 14, 'count': 1, 'stride': 1}, 'RENDER_FIELD_MASK': {'value': 127, 'count': 1, 'stride': 1}, 'RENDER_FIFO_DEPTH': {'value': 64, 'count': 1, 'stride': 1}, 'RENDER_FLUSH_OFF': {'value': 16392, 'count': 1, 'stride': 1}, 'RENDER_FULL_SHIFT': {'value': 21, 'count': 1, 'stride': 1}, 'RENDER_OCC_SHIFT': {'value': 18, 'count': 1, 'stride': 1}, 'RENDER_R10K_EMPTY': {'value': 0, 'count': 1, 'stride': 1}, 'RENDER_R10K_FULL': {'value': 20, 'count': 1, 'stride': 1}, 'RENDER_R10K_STALL': {'value': 19, 'count': 1, 'stride': 1}, 'RENDER_R5K_EMPTY': {'value': 24, 'count': 1, 'stride': 1}, 'RENDER_R5K_FULL': {'value': 48, 'count': 1, 'stride': 1}, 'RENDER_R5K_STALL': {'value': 47, 'count': 1, 'stride': 1}, 'RENDER_STALL_SHIFT': {'value': 7, 'count': 1, 'stride': 1}, 'RENDER_START_MASK': {'value': 63, 'count': 1, 'stride': 1}, 'RENDER_STATUS_IDLE_BIT': {'value': 28, 'count': 1, 'stride': 1}, 'RENDER_STATUS_OFF': {'value': 16384, 'count': 1, 'stride': 1}, 'RENDER_WPTR_SHIFT': {'value': 6, 'count': 1, 'stride': 1}, 'SOFTINT_OFF': {'value': 32, 'count': 1, 'stride': 1}, 'START_ALIAS_OFF': {'value': 2048, 'count': 1, 'stride': 1}, 'STENCIL_MASK_OFF': {'value': 8680, 'count': 1, 'stride': 1}, 'STENCIL_MODE_OFF': {'value': 8672, 'count': 1, 'stride': 1}, 'TEXTURE_MODE_OFF': {'value': 8464, 'count': 1, 'stride': 1}, 'TEX_LOG2_BYTES_SHIFT': {'value': 18, 'count': 1, 'stride': 1}, 'TEX_MAX_TEXELS': {'value': 1024, 'count': 1, 'stride': 1}, 'TEX_SUBTILE_BIT': {'value': 24, 'count': 1, 'stride': 1}, 'TEX_SUBTILE_CELL_BYTES': {'value': 32, 'count': 1, 'stride': 1}, 'TEX_TILE_FLAG': {'value': 32768, 'count': 1, 'stride': 1}, 'TEX_TLB_REGS': {'value': 28, 'count': 1, 'stride': 1}, 'TEX_TYPE_SHIFT': {'value': 20, 'count': 1, 'stride': 1}, 'TILE_BYTES': {'value': 65536, 'count': 1, 'stride': 1}, 'TILE_PAGES': {'value': 16, 'count': 1, 'stride': 1}, 'TILE_ROWS': {'value': 128, 'count': 1, 'stride': 1}, 'TILE_ROW_BYTES': {'value': 512, 'count': 1, 'stride': 1}, 'TIME_OFF': {'value': 56, 'count': 1, 'stride': 1}, 'TIME_RESOLUTION': {'value': 15000, 'count': 1, 'stride': 1}, 'VTX_DEPTH_BASE': {'value': 8650752, 'count': 1, 'stride': 1}, 'VTX_STEP': {'value': 64, 'count': 1, 'stride': 1}, 'VTX_TRI_BASE': {'value': 1208221696, 'count': 1, 'stride': 1}, 'WATCHDOG_OFF': {'value': 48, 'count': 1, 'stride': 1}}
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
