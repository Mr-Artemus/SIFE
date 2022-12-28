#![allow(non_snake_case)]   // Allow non snake case variables

use clap::{Parser};
use std::process::exit; 
use crate::algorithms::ALGORITHMS;

mod utils;
mod wipe;
mod algorithms;

// Generate the command line arguments
#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct Args {
    /// List available methods
    #[arg(short='l', long)]
    list: bool,

    /// Method to use
    #[arg(short='m', long)]
    method: Option<u16>,

    /// File to erase
    #[arg(short='f', long)]
    file: Vec<String>,

    /// Folder to erase
    #[arg(short='F', long)]
    folder: Vec<String>,

    /// Verbose mode
    #[arg(short='v', long)]
    verbose: bool,

    /// Keep the file after erasing
    #[arg(long)]
    keep: bool,
}

fn main() {
    // Parse the command line arguments
    let args: Args = Args::parse();

    // Print the arguments if verbose mode is enabled
    if args.verbose {
        println!("Parameters:");
        println!("  - List    : {:?}", args.list);
        println!("  - Method  : {:?}", args.method);
        println!("  - File    : {:?}", args.file);
        println!("  - Folder  : {:?}", args.folder);
        println!("  - Verbose : {:?}", args.verbose);
        println!("  - Keep    : {:?}", args.keep);
        println!("")
    }

    if args.list {
        println!("Available methods :");
        for i in 0..ALGORITHMS.len() {
            println!("  {} - {}", (i + 1), ALGORITHMS[i].name);
        }
        println!("\nPlease use the --method option to select a algorimthm")

    } else if args.file.len() > 0 || args.folder.len() > 0 {
        // Initialize the list of files to erase
        let mut listOfFiles: Vec<String> = Vec::new();
        let mut listOfNonWritableFiles: Vec<String> = Vec::new();

        // Add the files to the list
        for file in args.file {
            if utils::isRegularFile(file.clone()) {
                listOfFiles.push(file);
            } else {
                println!("[WARN] {} is not a regular file, skipping...", file);
            }
        }

        // Add the files in the folders to the list
        for folder in args.folder {
            if utils::isDirectory(folder.clone()) {
                listOfFiles.append(&mut utils::getFilesInDirectory(folder.clone()));
                listOfNonWritableFiles.append(&mut utils::getNonWritableFilesInDirectory(folder));
            } else {
                println!("[WARN] {} is not a directory, skipping...", folder);
                exit(1);
            }
        }
        println!("");

        // Warn the user if there are non-writable files
        if listOfNonWritableFiles.len() > 0 {
            println!("The following files are not writable, they will be skipped :");
            for file in listOfNonWritableFiles.clone() {
                println!("  - {}", file);
            }
        }

        if listOfFiles.len() > 0 {
            // List the files to erase if verbose mode is enabled
            if args.verbose {
                println!("Files to erase:");
                for file in listOfFiles.clone() {
                    println!("  - {}", file);
                }
                println!("");
            }

            // Wipe the files
            wipe::wipeFiles(
                listOfFiles,
                if args.method.is_none() { 1 } else { args.method.unwrap() },
                args.keep,
                args.verbose
            );
        } else {
            println!("[ERROR] There are no file to erase");
            exit(1);
        }
    } else {
        println!("No file or folder specified, please read the help");
        exit(1);
    }

    // Exit the program correctly
    exit(0);
}
