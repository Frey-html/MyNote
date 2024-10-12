In C++, lambda expressions provide a concise way to define anonymous functions (i.e., functions without names). They are useful when you need a short function that will only be used once or in a specific context, such as with algorithms, event handlers, or threading.

A **lambda expression** in C++ has the following general syntax:

```cpp
[capture](parameters) -> return_type {
    // function body
}
```

Let's break it down:

1. **[capture]**: Specifies which variables from the surrounding scope can be accessed inside the lambda.
   - `[ ]`: No variables are captured.
   - `[=]`: Capture all variables from the surrounding scope **by value** (i.e., make a copy).
   - `[&]`: Capture all variables from the surrounding scope **by reference**.
   - `[x]`: Capture the variable `x` by value.
   - `[&x]`: Capture the variable `x` by reference.
   - Mixed capture, e.g., `[x, &y]`: Capture `x` by value and `y` by reference.

2. **(parameters)**: The parameters passed to the lambda, just like in a normal function.

3. **-> return_type**: (Optional) Specifies the return type. If omitted, C++ will deduce the return type automatically, especially when the lambda contains only one return statement.

4. **function body**: The code that defines the behavior of the lambda function.

### Example 1: Simple Lambda Expression
Here is a basic example of a lambda expression that adds two numbers:

```cpp
#include <iostream>

int main() {
    // Simple lambda that adds two numbers
    auto add = [](int a, int b) -> int {
        return a + b;
    };

    std::cout << "Sum: " << add(5, 3) << std::endl;  // Output: 8
    return 0;
}
```

In this example:
- `[ ]` means no variables are captured.
- `(int a, int b)` are the parameters.
- `-> int` specifies the return type, which is `int` in this case.

### Example 2: Lambda with Capturing Variables
Here's how you can capture variables from the surrounding scope:

```cpp
#include <iostream>

int main() {
    int x = 10;
    int y = 20;

    // Capture x by value and y by reference
    auto lambda = [x, &y]() {
        std::cout << "x: " << x << ", y: " << y << std::endl;
        // You can modify y, but x is captured by value so it can't be changed
        y = 50;
    };

    lambda();  // Output: x: 10, y: 20
    std::cout << "Modified y: " << y << std::endl;  // Output: Modified y: 50

    return 0;
}
```

In this example:
- `[x, &y]`: Captures `x` by value (you can't modify `x` inside the lambda) and `y` by reference (you can modify `y` inside the lambda).
- The lambda prints the values of `x` and `y`, and modifies `y`.

### Example 3: Lambda with Standard Algorithms
Lambdas are commonly used with STL algorithms like `std::sort` or `std::for_each`. Here's an example of using a lambda with `std::sort`:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {5, 2, 8, 1, 3};

    // Sort the vector in descending order using a lambda
    std::sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a > b;
    });

    // Output the sorted vector
    for (int num : numbers) {
        std::cout << num << " ";  // Output: 8 5 3 2 1
    }

    return 0;
}
```

In this example:
- `[](int a, int b) { return a > b; }`: A lambda that compares two numbers to sort them in descending order.
- The lambda is passed as the third argument to `std::sort`, which defines the comparison logic.

### Example 4: Generic Lambda (C++14 and Later)
From C++14 onward, lambdas can have auto-typed parameters, making them more flexible (i.e., **generic lambdas**).

```cpp
#include <iostream>

int main() {
    // Generic lambda that works with different types
    auto print = [](auto x) {
        std::cout << x << std::endl;
    };

    print(42);      // Output: 42
    print(3.14);    // Output: 3.14
    print("Hello"); // Output: Hello

    return 0;
}
```

In this example:
- `auto` allows the lambda to work with parameters of any type, making it a generic function.

### Example 5: Mutable Lambdas
By default, lambdas that capture variables by value cannot modify them. However, if you want to modify captured variables by value, you can use the `mutable` keyword:

```cpp
#include <iostream>

int main() {
    int x = 10;

    // Capture x by value, but allow modification within the lambda
    auto lambda = [x]() mutable {
        x += 5;
        std::cout << "Modified x inside lambda: " << x << std::endl;
    };

    lambda();  // Output: Modified x inside lambda: 15
    std::cout << "Original x outside lambda: " << x << std::endl;  // Output: Original x outside lambda: 10

    return 0;
}
```

In this example:
- `mutable` allows `x` to be modified inside the lambda, even though it is captured by value.
- Outside the lambda, `x` remains unchanged because the capture was by value, and the changes were local to the lambda.

### Summary of Key Concepts
- **Capture variables**: Choose whether to capture by value or reference.
- **Parameters and return types**: Use them just like normal functions.
- **Lambda expressions** can be used in many contexts, including algorithms and callbacks.
- **`mutable` lambdas** allow modifying captured variables by value.
- **Generic lambdas** with `auto` parameters are a powerful feature introduced in C++14.

### Example: Using lambdas with `std::for_each`

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Using lambda in for_each
    std::for_each(v.begin(), v.end(), [](int &n) {
        n *= 2;
    });

    for (int n : v) {
        std::cout << n << " ";  // Output: 2 4 6 8 10
    }

    return 0;
}
```

This shows how a lambda can be used to modify elements in a container using `std::for_each`.

Lambdas are a powerful and flexible feature in C++, allowing you to write cleaner and more concise code, especially when used with algorithms or event-driven programming.