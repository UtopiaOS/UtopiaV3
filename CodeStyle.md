# Code style

Utopia is a project about _integration_ and _unification_ we believe your code is better if it follows the same style the last developer used. Please review this document entirely 
in order to ensure your code is the best as it could be for the Utopia project. Thank you for your cooperation.

#### Table Of Contents

- [C](#C)
- [Rust](#Rust)

# C

The following guidelines refer to the C programming language, and are based on code by the BSD and Linux projects.

## Typing conventions

- Use `snake_case` for variables, for example:

```c
int my_variable = 0;
```

- Use `PascalCase` for Structs and Unions, for example: 

```c
struct HelloWorld {
  char* foo;
  int bar;
}

union GoodbyeWorld {
  char* faa;
  int bor;
}
```

- Use `snake_case` for functions, for example
```c
int 
do_something(int argument) 
{
    int something = 0;
    
    something = argument + 1;
    
    return something;
}
```

## Naming conventions

- Use descriptive action or propety verbs when possible, for example:

**Checking if a propety or condition value falls into two categories**

**Variables**

In the following code, we want to check if the _user_ has a certain _property_ in this case, the property is whether the user _is_ root or not.

```c
bool is_root = check_root(user);
```
In another example, we use the verb _do_ to check if we should or shouldn't execute an action.

```c
bool do_a_barrel_roll = should_do_barrel();
```

**Functions**

When you want to read a _value_ you might want to use the _check_ verb as the first letter in your function name, for example:


The following code, shows an example of _check_ type action

```c
bool 
check_root(user_t *user) 
{  
  if (user_t == NULL) 
      return false;
  
   return (user->id == ROOT_USER_ID) ? true : false;
}


