# What is this?
This is a C-code generator to leverage [LT-Codes](https://en.wikipedia.org/wiki/Luby_transform_code)
in your C program. The project itself is written in Python. Based on your input, a C source and header
file named `lt2c.c` and `lt2c.h` will be generated. You can copy them in your own project and
use `lt2c_encode` and `lt2c_decode` functions.

I tend to use the term `Symbol` for input data, and `Drop` for
output data. So we encode Symbols to generate Drops, then we decode Drops to recover Symbols.
Theroritically we can generate infinite number of Drops. When the decoder receives number of drops
slightly greather than number of symbols, it will be able to decode successfully. It doesn't
matter in what order the drops are received, just quantity of them is important. (Albeit it depens on how well
the encoding matrix is generated).


# Mandatory input arguments
1. Number of input symbols
2. Number of output drops
3. Block size

For implementation reasons, input data size is limited. Input data size is made up by number of symbols multiply by
block size. You have to define them both.
You can have up to 64 symbols, but block size is unlimited. However, you should know that if a block of data gets corrupted,
the whole thing must be thrown away. There is no way to recover a corrupted block. Hence, you best keep the block size
as small as possible.

For example, if you want to encode 128 bytes of data, you may define `16` symbols of size `8`. You can generate as many drops
as you want. Let's add 100% redundancy, so we would like to get 32 drops in the output.
Following command line can be used to achieve this:

```python
python lt2c.py 16 32 8
```

## Optional input arguments
Generating an encoding matrix involves randomness. There are technoquies to make it more robust, but still it relies on
random data. You may get a more robust matrix if you keep generating and verifying it. This is what I did.
There are some input arguments to control how strict you want the validation process to be.
The script generates some random data and tries to encode/decode the data given an encoding matrix. It does it for number
of times and verify decode success rate. This is the process it follows:

1. Generate some random data
2. Generate an encoding matrix
3. Dropout input data with probability `p`
2. Encode the input data
3. Check if decode is successfull
4. Loop from step 3 for multiple times (called trials)
5. Compute success rate. Go to step 1 of it doesn't satisfy requiremtns
6. Finally generates C source based on generated matrix

`--dropout` or `-d` controls dropout probability `p`. The range is from `0` to `P` where `P` is the maxumim possible
probability such that number of encoded drops is still greather than number of input symbols (Therorically can't be otherwise).

`--success` or `-s` defines your desired success rate. Should be beween 0.5 and 1.0 where 1.0 means you want 100% successful decodes.

`--trials` or `-t` defines number of itertations from step 3. Default is 100.

If your requrements are satisfied, you will get C source files generated and a report of maximum success achieved during the process.
Otherwise, it doesn't create any output source file and only reports the maximum success rate. You may retry with less strict
requrements.

The output source files will be placed in the current directory by default. You can define an output directory using `--output` or `-o` switch.

Here is the overview of all available switches:

`lt2c [-h] [-t TRIALS] [-s SUCCESS] [-d DROPOUT] [-o OUT] symbols drops block`

# Example code
You can find an example in the [example](example) directory. The important parts of the example are described below:

```c
uint8_t data1[LT2C_SYMBOLS_SZ] = {1, 2, 3, 4, 5, 6, 7, 8};
uint8_t data2[LT2C_SYMBOLS_SZ];
uint8_t drops[LT2C_DROPS_SZ];
  
lt2c_encode(drops, data1);

/* Simulate a noisy channel by making some of the drops available */
uint32_t st[LT2C_STATUS_SZ] = {0x030F1F07};
int ret = lt2c_decode(data2, drops, st);

if (ret == 0) {
  /* Successful decode */
}
```

It encodes a sample data `data1` and store the result in `drops`. Note that how `LT2C_SYMBOL_SZ` macro
is used which is already defined in `lt2c.h`. It helps you to safely define your input symbol arrays.
There is also another macro that defines drop array size: `LT2C_DROPS_SZ`.

Next step is to use `lt2c_decode` function. It tries to decode from a subset of blocks in `drops` and store the
result in `data2`.
The subset is defined in variable `st` which is a bitmask variable such that bit #0 corresponds to the first
block of data, bit #1 is for second block of data and so on. If a bit is set, it means that block is
available. Imagine an RF receiver. Every time it receives a block of data, it stores the block within
`drops` array and set the correspoing flag in `st` status variable, so the decoder will know what blocks
are available to use.

Note that when you receive a drop, you should store it in right place within `drops` array.
You can specify a packet layout and for example assign the first byte to indicate drop's index number.
Then multiply this number by block size to get to the correct address within `drops` array.
I didn't implement this in this project to keep things simple.

Also note that variable `st` is an array with a variable size depending on how many bits are needed.
A helper macro named `LT2C_STATUS_SZ` is defined in `lt2c.h` to be used to create this variable.
Drop number 0 is the least significant bit of least significant byte, and so on.
You may use the following piece of code to set the corresponding bit:

```c
uint8_t drop_idx = 40; // For example we received a drop with index 40
uint8_t bytenum = drop_idx / 32;
uint8_t bitnum = drop_idx % 32;
st[bytenum] |= (1<<bitnum);
```