# Instrukcije za parser
(Sve je isto kao i C verzija osim sto pojedine datoteke imaju .cpp ekstenziju umjesto .c)

(instrukcije kopirane iz ovog repozitorija https://github.com/IvanVnucec/fersat-parser)

Fersat-parser is command line parser which is used for testing purposes for small 1U Cubesat which is being developed at the University of Zagreb ([Link](https://www.fer.unizg.hr/zrk/o_zavodu/studenti/cubesat/FERSat)).

Parser works by reading a string for example `set_fan_speed(20)` and it will call a system function based on function name, it this case `set_fan_speed`. 
The number of arguments between paranteses depends on your needs. There could be 0 or more arguments. 

## Getting Started

### Installing

* Put all .c and .h files into your project. 
* Add your own functions (see Deployment below)

## Running the tests

Call parser by typing
```
parse(string);
```
where `string` is a pointer on some C style string (ending with the '\0' sign). `string` contains a function which will be called based on the function name.
If the function name is correct and the number of arguments is sufficient, parser should call the function. 

## Deployment

To use the parser one must do the following:
1. Create wrapper function
2. Creating a caller for the original function

#### 1. Create wrapper function

Wrapper function is function which connects your system function which you want to call and the parser itself. 

Wrapper function must be placed into the `wrapper.c` file!
Declaration of every wrapper function must be placed into the `wrapper.h` file.


Input of every Wrapper function is pointer to the linked list of the function arguments `list_t *arg`.

Output of every Wrapper function is void.

Every wrapper function has the following definition:
```
void wrapp_fun_name(list_t *arg)
```
where `list_t *arg` is the queue for the function arguments. They are represented as the standard C-type string.

What every Wrapper function has:
1. Allocating memory for the function arguments  (OPTIONAL)
2. Copying the function arguments into allocated memory  (OPTIONAL)
3. Converting function arguments from type `string` to the other types (`int`,`float` etc.)  (OPTIONAL)
4. Calling of the original function  (MANDATORY)
5. Freeing allocated memory  (OPTIONAL)

Some examples of wrapper function:
```
void set_fan_speed_wrapper(list_t *arg){
    // 1. Allocating memory for the function arguments
    char *arg1 = malloc(BUFFER_SIZE);

    // 2. Copying the function arguments into allocated memory
    list_remove(arg, arg1, BUFFER_SIZE);
    
    // 3. Converting function arguments from type string to the other types (int,float etc.)
    fan_speed = str_to_int(arg1);

    // 4. Calling of the original function
    set_fan_speed(fan_speed);

    // 5. Freeing allocated memory
    free(arg1);
}
```

```
void led_on_wrapp(list_t *arg) {
  // 4. Calling of the original function 
  led_on();
}
```

#### 2. Creating a caller for the original function

One must modify a `function_wrapper_init` function which is located in `wrapper.c` file.

Parser for every wrapper function must know the function name, the number of arguments which every wrapper function takes and the pointer to the original function.

For the two wrapper functions above the `function_wrapper_init` looks like:

```
int function_wrapper_init (function_list_t *function_list) {
  
    if(function_list_add(function_list,  "set_fan_speed",  1, set_fan_speed_wrapper))  return 1;  // ERROR
    if(function_list_add(function_list,  "led_on",         0, led_on_wrapp))           return 1;  // ERROR

    return 0;   // Return everything OK
}
```

The second argument of function `function_list_add` is the function name on which parser will 'react', the third is the number of arguments.
If the number of arguments is not exactly that number, the parser will not call the original function. Fourth one is the pointer to a wrapper function.

In `wrapper.h` file, there should be included a header files in which there are declarations of the original functions. 

## Authors

* **Ivan Vnučec** - [github](https://github.com/IvanVnucec), [Linkedin](https://www.linkedin.com/in/ivan-vnu%C4%8Dec-b5b183187/)

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/IvanVnucec/fersat-parser/blob/master/LICENSE) file for details.
