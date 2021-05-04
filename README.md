# Excel array functions

Excel has a data type for two dimensional arrays of floating point numbers.
This data type is only available for the ancient Excel C SDK but it provides
much higher performance than the C#/VBA/Javascript APIs.
A pointer to the array of numbers can be passed to C functions without
copying the array.
This library can create and manipulate in-memory arrays that have the same performance as native code.

Most array functions can take either an array or a handle to an in-memory array.
If a handle is given then the function operates on the in-memory array and returns the same handle.
This breaks Excel's purely functional convention so you may need to specify dependencies explicitly.
The library has a function `DEPENDS(cell, dependency)` that forces `dependency` to be
calculated before returning `cell`. This is only useful if `dependency` is a cell reference.
