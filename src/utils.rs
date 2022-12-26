use std::fs::{Metadata, metadata, read_dir};
use std::io::Error;

/// Function to check if a path is a regular file.
/// 
/// # Arguments
/// * `path` - Path to check
/// 
/// # Returns
/// * `boolean` - true if the path is a regular file, false otherwise
/// 
/// # Example
/// ```
/// let path: String = "file.txt";
/// let is_file: bool = isRegularFile(path);
/// ```
pub fn isRegularFile(path: String) -> bool {
    let metadata: Result<Metadata, Error> = metadata(path);

    return metadata.is_ok() && metadata.unwrap().is_file();
}

/// Function to check if a path is a directory.
/// 
/// # Arguments
/// * `path` - Path to check
/// 
/// # Returns
/// * `boolean` - true if the path is a directory, false otherwise
/// 
/// # Example
/// ```
/// let path: String = "myFolder";
/// let is_dir: bool = isDirectory(path);
/// ```
pub fn isDirectory(path: String) -> bool {
    let metadata: Result<Metadata, Error> = metadata(path);

    return metadata.is_ok() && metadata.unwrap().is_dir();
}

/// Function to check if a path is writable.
/// 
/// # Arguments
/// * `path` - Path to check
/// 
/// # Returns
/// * `boolean` - true if the path is writable, false otherwise
/// 
/// # Example
/// ```
/// let path: String = "file.txt";
/// let is_writable: bool = isWritable(path);
/// ```
pub fn isWritable(path: String) -> bool {
    let metadata: Result<Metadata, Error> = metadata(path);

    return metadata.is_ok() && metadata.unwrap().permissions().readonly() == false;
}

/// Function that returns all files in a directory recursively.
/// 
/// # Arguments
/// * `path` - Path to check
/// 
/// # Returns
/// * `Vec<String>` - Vector of files in the directory
/// 
/// # Example
/// ```
/// let path: String = "myFolder";
/// let files: Vec<String> = getFilesInDirectory(path);
/// ```
pub fn getFilesInDirectory(path: String) -> Vec<String> {
    if isDirectory(path.clone()) {
        let mut files: Vec<String> = Vec::new();

        for path in read_dir(path).unwrap() {
            let path: String = path.unwrap().path().to_str().unwrap().to_string();
            if isRegularFile(path.clone()) {
                files.push(path);
            } else if isDirectory(path.clone()) {
                files.append(&mut getFilesInDirectory(path));
            }
        }

        return files;
    } else {
        return Vec::new();
    }
}

/// Function that returns all non-writable files in a directory recursively.
/// 
/// # Arguments
/// * `path` - Path to check
/// 
/// # Returns
/// * `Vec<String>` - Vector of non-writable files in the directory
/// 
/// # Example
/// ```
/// let path: String = "myFolder";
/// let files: Vec<String> = getNonWritableFilesInDirectory(path);
/// ```
pub fn getNonWritableFilesInDirectory(path: String) -> Vec<String> {
    if isDirectory(path.clone()) {
        let mut files: Vec<String> = Vec::new();

        for path in read_dir(path).unwrap() {
            let path: String = path.unwrap().path().to_str().unwrap().to_string();
            if isRegularFile(path.clone()) {
                if !isWritable(path.clone()) {
                    files.push(path);
                }
            } else if isDirectory(path.clone()) {
                files.append(&mut getNonWritableFilesInDirectory(path));
            }
        }

        return files;
    } else {
        return Vec::new();
    }
}