# Excel array functions

Excel has a data type for two dimensional arrays of floating point numbers.
This data type is only available for the ancient Excel C SDK but it provides
much higher performance than the C#/VBA/Javascript APIs.
A pointer to the array of numbers can be passed to C functions without copying the array.
This library can create and manipulate in-memory arrays that have the same performance as native code.
It can handle 2-d arrays upto 2^31 x 2^31.

Most array functions can take either an array or a handle to an in-memory array.
If a handle is given then the function operates on the in-memory array and returns the same handle.
This breaks Excel's function dependency conventions so you may need to specify dependencies explicitly.
The library has a function `DEPENDS(cell, dependency)` that forces `dependency` to be
calculated before returning `cell`.

## `ARRAY`

The function `\ARRAY(array)` stores `array` in memory and returns a _handle_.
Use `ARRAY(handle)` to return the underlying array. The array dimensions
are given by `ARRAY.ROWS` and `ARRAY.COLUMNS`. The function `ARRAY.SIZE`
returns the product of rows and columns. Arrays can be resized using
`ARRAY.RESIZE(array, rows, columns)`.

## `INDEX`

Select array elements with `ARRAY.INDEX(array, rows, columns)` where `rows`
and `columns` are the indices to select. Indices are modulo the size
of the dimension so, for example, `ARRAY.INDEX(array, -1)` selects the
last element of `array`. If `rows` or `columns` are missing then all
rows or columns are returned.

## `TAKE`, `DROP`

The function `ARRAY.TAKE(array, count)` takes `count` items from the beginning of
the array if `count > 0` and `-count` items from the end of the array if `count < 0`.
If `array` has more than one row then take operates on rows, for example,
`ARRAY.TAKE(array, -1)` returns the 1 x columns last row. Likewise for drop.
To return items corresponding to non-zero `mask` items call `ARRAY.MASK(array, mask)`.
If the mask size is different than the array size then the mask size must be equal
to the number of columns of `array` and the mask acts on its columns.

## `APPLY`

A unary function `f` can be applied to an array;
`ARRAY.APPLY(f, {a0, a1, a2, ...})` results in the array `{f(a0), f(a1), f(a2), ...}`.
If `f` is a string then it is assumed to be the name of an Excel unary function.

## `SCAN`

The _scan_ (or _right fold_) of an array using a binary operator `m` is
`{a0, m(a0, a1), m(a0, m(a1, a2), ...)}`. The add-in defines binary operators 
`ADD`, `SUB`, `MUL`, `DIV`, `MOD`, `MAX`, and `MIN`.
