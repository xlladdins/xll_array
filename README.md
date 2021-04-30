# Excel array functions

Excel has a data type for two dimensional arrays of floating point numbers.
This data type is only available for the ancient Excel C SDK but it provides
much higher performance than the C#/VBA/Javascript APIs.
A pointer to the array of numbers can be passed to C functions without
copying the array.
This library can create in-memory arrays that have the same performance as native code.
