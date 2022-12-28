use positioned_io::{WriteBytesAtExt, ReadBytesAtExt};
use std::fs::{File, OpenOptions, remove_file};
use std::process::exit;
use std::io::{Error, stdout, Write};
use crate::algorithms::ALGORITHMS;

// Algorithm struct
pub struct Algorithm {
    pub name: &'static str,
    pub(crate) writes: &'static [fn() -> u8],
    pub(crate) checks: &'static [fn(u8) -> bool],
}

static CLEAR_LINE: &'static str = "\x1b[2K\r";

pub fn wipeFiles(files: Vec<String>, algorithmNbr: u16, keep: bool, verbose: bool) {
    // Check if the algorithm number is valid
    if algorithmNbr < 1 || algorithmNbr > ALGORITHMS.len() as u16 {
        println!("Invalid algorithm number, please use the --list option to see the available algorithms");
        exit(1);
    }

    // Get the algorithm
    let algorithm: &Algorithm = &ALGORITHMS[(algorithmNbr - 1) as usize];

    // Print the algorithm name if verbose mode is enabled
    if verbose {
        println!("Using algorithm: {}", algorithm.name);
        println!("");
    }

    // For each file
    let totalFiles: u64 = files.len() as u64;
    for (index, file) in files.iter().enumerate() {
        let index: u64 = (index + 1) as u64;
        if crate::utils::isRegularFile(file.clone()) {
            if crate::utils::isWritable(file.clone()) {
                print!("Opening {} ...", file);
                stdout().flush().ok();

                // Open the file stream
                let fp: Result<File, Error> = OpenOptions::new().read(true).write(true).open(file.clone());

                // If the file is open
                if fp.is_ok() {
                    let mut fp: File = fp.unwrap();
                    let fileSize: u64 = fp.metadata().unwrap().len();
                    let mut offset: u64;
                    let mut percent: f32 = 0.00;
                    let mut writeError: bool = false;
                    let mut checkError: bool = false;

                    // Print the percent
                    print!("{}[file {}/{}][  0.00%] - {}", CLEAR_LINE, index, totalFiles, file);
                    stdout().flush().ok();

                    // For each write function
                    for i in 0..algorithm.writes.len() as u64 {
                        // Reset offset
                        offset = 0;

                        // For each byte write
                        for ii in 0..fileSize {
                            // Calculate the percent
                            let mut newPercent: f32 = 100.0 * (((i * fileSize * 2) + ii) as f32) / (algorithm.writes.len() as u64 * fileSize * 2) as f32;
                            newPercent = f32::trunc(newPercent  * 100.0) / 100.0;

                            // Print the percent
                            if newPercent != percent {
                                percent = newPercent;
                                print!("{}[file {}/{}][{: >6.2}%] - {}", CLEAR_LINE, index, totalFiles, percent, file);
                                stdout().flush().ok();
                            }

                            // Write the byte
                            let writeResult: Result<(), Error> = fp.write_u8_at(offset, algorithm.writes[i as usize]());
                            if writeResult.is_err() {
                                writeError = true;
                                println!("{}An error as occured while writing {} :", CLEAR_LINE, file);
                                println!("\t{}", writeResult.err().unwrap());
                                break;
                            }

                            // Set offset to the next position
                            offset += 1;
                        }

                        // Print the percent
                        //print!("{}[{}] - {}", CLEAR_LINE, if checkError { " ERROR " } else { " 50.00%" }, file);
                        //stdout().flush().ok();

                        // Reset offset
                        offset = 0;

                        // For each byte check
                        for ii in 0..fileSize {
                            // If an error has occured while writting skip
                            if writeError {
                                break;
                            }

                            // Calculate the percent
                            let mut newPercent: f32 = 100.0 * (((i * fileSize * 2) + ii) as f32 + fileSize as f32) / (algorithm.writes.len() as u64 * fileSize * 2) as f32;
                            newPercent = f32::trunc(newPercent  * 100.0) / 100.0;

                            // Print the percent
                            if newPercent != percent {
                                percent = newPercent;
                                print!("{}[file {}/{}][{: >6.2}%] - {}", CLEAR_LINE, index, totalFiles, percent, file);
                                stdout().flush().ok();
                            }

                            // Read the byte
                            let readResult: Result<u8, Error> = fp.read_u8_at(offset);

                            // Check the byte
                            if readResult.is_err() {
                                checkError = true;
                                println!("{}An error as occured while checking {} :", CLEAR_LINE, file);
                                println!("\t{}", readResult.err().unwrap());
                                break;
                            } else {
                                if !algorithm.checks[i as usize](readResult.unwrap()) {
                                    checkError = true;
                                    println!("{}An error as occured while checking {} :", CLEAR_LINE, file);
                                    println!("\tThe byte at offset {} is not the expected one", offset);
                                    break;
                                }
                            }

                            // Set offset to the next position
                            offset += 1;
                        }

                        // Print the percent
                        print!("{}[file {}/{}][{}] - {}", CLEAR_LINE, index, totalFiles, if checkError { " ERROR " } else { "100.00%" }, file);
                        stdout().flush().ok();
                    }

                    // Delete the file if keep is false
                    if !keep {
                        if verbose {
                            print!("\nDeleting {} ...", file);
                        }
                        let deleteResult: Result<(), Error> = remove_file(file.clone());
                        if deleteResult.is_err() {
                            println!("\nAn error as occured while deleting {} :", file);
                            println!("\t{}", deleteResult.err().unwrap());
                        }
                    }

                    println!("");
                    stdout().flush().ok();

                } else {
                    println!("{} is not readable, skipping...", file);
                }
            } else {
                println!("{} is not writable, skipping...", file);
            }
        } else {
            println!("{} is not a regular file, skipping...", file);
        }
    }
}