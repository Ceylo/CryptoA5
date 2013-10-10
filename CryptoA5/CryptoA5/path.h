//
//  path.h
//  CryptoA5
//
//  Created by Ceylo on 10/10/2013.
//  Copyright (c) 2013 Yalir. All rights reserved.
//

#ifndef CryptoA5_path_h
#define CryptoA5_path_h

/** Call this function once before any call to PathForFile
 *
 * @param executablePath the full path to the current executable
 * (usually argv[0])
 */
void PathInit(const char *executablePath);

/** Create a absolute path for the given file name,
 * relative to the executable's directory
 *
 * @param path the relative path you want to make absolute
 * @param absolutePath a reference to a (char *) that will
 * reference the resulting absolute path.
 * /!\ WARNING: You're responsible for freeing that string. /!\
 */
void PathForFile(const char *path, char **absolutePath);

#endif
