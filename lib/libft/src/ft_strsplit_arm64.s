.global _ft_strsplit
.extern _malloc

// Function that breaks a given input string using a given delimiter.
// Arg1 (X0): Input string (char *)
// Arg2 (X1): Deliminter (char)
// Returns: A NULL terminated array of pointers, pointing to the split string segments.
// The returned pointer should be freed, but the pointers in the array should NOT be free.
_ft_strsplit:
    stp x29, x30, [sp, #-16]! // Save LR and SP
    mov X9, #0                // String length
    mov X10, #0               // Num words
count_words:
    ldrb W15, [X0], #1        // Load next character of string into W15
    cmp W15, W1               // Is it the delimiter?
    bne not_delim
    add X10, X10, #1          // Increment word count
not_delim:
    add X9, X9, #1            // Increment character count (computing strlen)
    cbnz W15, count_words     // Reached end of string?
calc_mem:
    sub X0, X0, X9      // Rewind X0 back to start of string by subtracting strlen.
    add X10, X10, #2    // Add 2 to X10 to get total number of words (last word + NULL terminator)
    lsl X11, X10, #3    // Multiple by 8 (sizeof ptr) to get space needed for pointer array
    add X12, X11, X9    // Add length of string to that to get size of memory to allocate.
alloc_mem:
    // Stash registers that might get clobbered.
    stp X0, X1, [SP, #-16]!
    stp X9, X10, [SP, #-16]!
    str X11, [SP, #-16]!
    mov X0, X12    // Pass the memory size to malloc (X0 is the first argument)
    bl _malloc     // Allocate memory
    cbz X0, ret    // If it's NULL, return
    mov X12, X0    // Move allocated mem to X12.
    // Restore stashed registers.
    ldr X11, [SP], #16
    ldp X9, X10, [SP], #16
    ldp X0, X1, [SP], #16
build_array:
    mov X14, X12         // Save start of memory to X14, so we can return it later
    add X11, X12, X11    // Set X11 to point to location after pointer array (first word)
build_word:
    str X11, [X12], #8   // Save the address of the start of the word to the pointer array, and auto increment
copy_word:
    ldrsb W15, [X0], #1    // Read the next character from the input string
    cbz W15, end_of_array  // End of string?
    cmp W15, W1            // Delimiter?
    beq end_of_word
    strb W15, [X11], #1    // Append the character to the current word's location, and auto increment
    b copy_word            // Next character
end_of_word:
    strb WZR, [X11], #1    // Terminate the current word with a NULL
    b build_word           // Copy the next work
end_of_array:
    strb WZR, [X11]        // Null terminate the last word
    str XZR, [X12]         // Null terminate the pointers array
    mov X0, X14            // Move the start of the pointer array to X0 as return value.
ret:
    ldp x29, x30, [sp], #16  // Restore LR and SP
    ret

