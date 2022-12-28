use crate::wipe::Algorithm;

// Writting internal functions for wipe
fn _write0          ()  -> u8 { return 0b00000000 }
fn _write1          ()  -> u8 { return 0b11111111 }
fn _write01         ()  -> u8 { return 0b01010101 }
fn _writeRandom     ()  -> u8 { return rand::random::<u8>() }

// Checkings internal functions for wipe
fn _check0          (byte: u8)  -> bool { return byte == 0b00000000 }
fn _check1          (byte: u8)  -> bool { return byte == 0b11111111 }
fn _check01         (byte: u8)  -> bool { return byte == 0b01010101 }
fn _checkRandom     (_byte: u8) -> bool { return true }

// Algorithm list
pub static ALGORITHMS: &[Algorithm] = &[
    Algorithm {
        name: "Simple pass of 0b00000000",
        writes: &[_write0],
        checks: &[_check0]
    },
    Algorithm {
        name: "Simple pass of 0b11111111",
        writes: &[_write1],
        checks: &[_check1]
    },
    Algorithm {
        name: "Simple pass of 0b01010101",
        writes: &[_write01],
        checks: &[_check01]
    },
    Algorithm {
        name: "Simple pass of pseudo-random byte",
        writes: &[_writeRandom],
        checks: &[_checkRandom]
    },
    Algorithm {
        name: "GOST-R-50739-95 first version",
        writes: &[_write0, _writeRandom],
        checks: &[_check0, _checkRandom]
    },
    Algorithm {
        name: "GOST-R-50739-95 second version",
        writes: &[_writeRandom],
        checks: &[_checkRandom]
    },
    Algorithm {
        name: "HMG IS5",
        writes: &[_write0, _writeRandom],
        checks: &[_check0, _checkRandom]
    },
    Algorithm {
        name: "HMG IS5 Enhanced",
        writes: &[_write0, _write1, _writeRandom],
        checks: &[_check0, _check1, _checkRandom]
    },
    Algorithm {
        name: "RCMP TSSIT OPS-II",
        writes: &[_write0, _write1, _write0, _write1, _write0, _write1, _writeRandom],
        checks: &[_check0, _check1, _check0, _check1, _check0, _check1, _checkRandom]
    },
    Algorithm {
        name: "AFSSI-5020",
        writes: &[_write0, _write1, _writeRandom],
        checks: &[_check0, _check1, _checkRandom]
    },
    Algorithm {
        name: "AR 380-19",
        writes: &[_writeRandom, _write1, _write0],
        checks: &[_checkRandom, _check1, _check0]
    },
    Algorithm {
        name: "Bruce Schneier Algorithm",
        writes: &[_write0, _write1, _writeRandom, _writeRandom, _writeRandom, _writeRandom, _writeRandom],
        checks: &[_check0, _check1, _checkRandom, _checkRandom, _checkRandom, _checkRandom, _checkRandom]
    }
];