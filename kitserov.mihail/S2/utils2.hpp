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

      queue.push(token);
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
  Queue< T > infixToPostfix(Queue< T >& infix)
  {
      Queue< T > postfix;
      Stack< T > operations;

      while (!infix.isEmpty())
      {
          T token = infix.drop();

          if (token == "(")
          {
              operations.push(token);
          }
          else if (token == ")")
          {
              bool foundOpen = false;
              while (!operations.isEmpty())
              {
                  T op = operations.drop();
                  if (op == "(")
                  {
                      foundOpen = true;
                      break;
                  }
                  postfix.push(op);
              }
              if (!foundOpen)
              {
                  throw std::logic_error("Invalid expression: no matching opening bracket");
              }
          }
          else if (isOperation(token))
          {
              while (!operations.isEmpty())
              {
                  T top = operations.drop();
                  if (isOperation(top) && getOperationPriority(top) >= getOperationPriority(token))
                  {
                      postfix.push(top);
                  }
                  else
                  {
                      operations.push(top);
                      break;
                  }
              }
              operations.push(token);
          }
          else
          {
              postfix.push(token);
          }
      }
      while (!operations.isEmpty())
      {
          T op = operations.drop();
          if (op == "(" || op == ")")
          {
              throw std::invalid_argument("Mismatched parentheses");
          }
          postfix.push(op);
      }

      return postfix;
  }
  template < class T >
  void printQueue(Queue< T >& queue)
  {
    printList(queue.inList());
  }
  template < class T >
  T calculatePostfix(Queue< std::string >& queue)
  {
    Stack< T > stack;
    while (!queue.isEmpty()) {
      std::string token = queue.drop();
      bool isNumber = false;
      if (!token.empty()) {
          size_t start = (token[0] == '-') ? 1 : 0;
          if (start < token.size()) {
              isNumber = true;
              for (size_t i = start; i < token.size(); ++i) {
                  if (!std::isdigit(static_cast< unsigned char >(token[i]))) {
                      isNumber = false;
                      break;
                  }
              }
          }
          if (start == token.size()) {
             isNumber = false;
          }
      }

      if (isNumber) {
      T value;
      if constexpr (std::is_same_v<T, int>) {
          value = std::stoi(token);
      } else if constexpr (std::is_same_v<T, double>) {
          value = std::stod(token);
      } else {
          throw std::invalid_argument("Unsupported type");
      }
      stack.push(value);
      } else if (isOperation(token)) {
        if (stack.isEmpty()) {
          throw std::logic_error("Not enough operands for operation " + token);
        }
        T r = stack.drop();
        if (stack.isEmpty()) {
          throw std::logic_error("Not enough operands for operation " + token);
        }
        T l = stack.drop();
        T result = 0;
        if (token == "+") {
          result = l + r;
        } else if (token == "-") {
          result = l - r;
        } else if (token == "*") {
          result = l * r;
        } else if (token == "/") {
          if (r == 0) {
            throw std::logic_error("Division by zero");
          }
          result = l / r;
        } else if (token == "%") {
          if (r == 0) {
            throw std::logic_error("Modulo by zero");
          }
          result = l % r;
        }
        stack.push(result);
      } else {
        throw std::invalid_argument("Invalid token: " + token);
      }
    }
    if (stack.isEmpty()) {
      throw std::logic_error("Empty expression");
    }
    T result = stack.drop();
    if (!stack.isEmpty()) {
      throw std::logic_error("Too many values left on stack");
    }
    return result;
  }
}

#endif