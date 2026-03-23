#ifndef UTILS2_HPP
#define UTILS2_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace kitserov
{
  template< class T >
  Queue< T > readLine(std::istream& in, bool& isEof)
  {
    isEof = false;
    Queue< T > queue;
    T token;
    while (true) {
      const int nextChar = in.peek();
      if (nextChar == EOF) {
          isEof = true;
        break;
      }
      if (nextChar == '\n') {
        in.get();
        break;
      }

      if (!(in >> token)) {
        break;
      }

      queue.push(static_cast< T >(token));
    }
    return queue;
  }

  int getOperationPriority(const std::string& operation)
  {
    if (operation == "+" || operation == "-") {
      return 1;
    }
    if (operation == "*" || operation == "/" || operation == "%") {
      return 2;
    }
    return 0;
  }

  bool isOperation(const std::string& token)
  {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%";
  }

  template< class T >
  Queue< T > infixToPostfix(Queue< T > infix)
  {
    std::cout << "Start func\n";
    Queue< T > postfix;
    std::cout << "postfix";
    Stack< T > operations;
    std::cout << "stack\n";
    while (true) {
      T token;
      try {
        token = infix.drop();
      } catch (const std::out_of_range&) {
        break;
      }

      const std::string tokenString = token;
      if (tokenString == "(") {
        operations.push(token);
        continue;
      }

      if (tokenString == ")") {
        bool hasOpenBracket = false;
        while (true) {
          T operationToken;
          try {
            operationToken = operations.drop();
          } catch (const std::out_of_range&) {
            break;
          }

          const std::string operationString = operationToken;
          if (operationString == "(") {
            hasOpenBracket = true;
            break;
          }
          postfix.push(operationToken);
        }

        if (!hasOpenBracket) {
          throw std::logic_error("Invalid expression: no matching opening bracket");
        }
        continue;
      }

      if (!isOperation(tokenString)) {
        postfix.push(token);
        continue;
      }

      const int tokenPriority = getOperationPriority(tokenString);
      while (true) {
        T operationToken;
        try {
          operationToken = operations.drop();
        } catch (const std::out_of_range&) {
          operations.push(token);
          break;
        }

        const std::string operationString = operationToken;
        if (operationString == "(") {
          operations.push(operationToken);
          operations.push(token);
          break;
        }

        const int operationPriority = getOperationPriority(operationString);
        if (operationPriority >= tokenPriority) {
          postfix.push(operationToken);
          continue;
        }

        operations.push(operationToken);
        operations.push(token);
        break;
      }
      std::cout << "3\n";
    }
    
    std::cout << "2\n";

    while (true) {
      T operationToken;
      try {
        operationToken = operations.drop();
      } catch (const std::out_of_range&) {
        break;
      }

      const std::string operationString = operationToken;
      if (operationString == "(" || operationString == ")") {
        throw std::logic_error("Invalid expression: mismatched brackets");
      }
      postfix.push(operationToken);
    }

    return postfix;
  }
}

#endif