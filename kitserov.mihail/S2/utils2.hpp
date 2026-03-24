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
}

#endif