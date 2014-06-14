#ifndef OBFUSCATE_H
#define OBFUSCATE_H

/*
	Obfuscate file contents.  This is not true encryption, but it does 
	prevent casual inspection.

	The DECRYPT(offset, c) macro maps from a file offset and encrypted byte 
	to the unencrypted value.  This touches every byte, try to make it fast!

	Obfuscated files use a special header to signal they need decoding.  The 
	header is not counted toward file size or seek location.
*/

#define CRYPT_HEADER "pz~obfc8"
#define CRYPT_HEADER_SIZE 8

// changed to plaintext to protect the innocent
// TODO: refactor obfuscation; move kernel out of this and make callback
#define DECRYPT(offset, c) c

#endif