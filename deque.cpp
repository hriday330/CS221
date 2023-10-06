/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque()
{
    data = {};
    n1 = 0;
    n2 = -1;
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    data.push_back(newItem);
    n2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T leftElement = data.at(n1);
    n1++;
    //if the data can all fit in the indices 0...n1-1
    if (n2 - n1 <= n1 - 1)
    {
        //resizing array
        vector<T> newData = {};
        for (int i = n1; i <= n2; i++)
        {
            newData.push_back(data.at(i));
        }
        //reset
        data = newData;
        n1 = 0;
        n2 = data.size() - 1;
    }
    return leftElement;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    T rightElement = data.at(n2);
    data.erase(data.begin() + n2);
    n2--;
    //if the data can all fit in the indices 0...n1-1
    if (n2 - n1 <= n1 - 1)
    {
        //resizing array
        vector<T> newData = {};
        for (int i = n1; i <= n2; i++)
        {
            newData.push_back(data.at(i));
        }
        //reset
        data = newData;
        n1 = 0;
        n2 = data.size() - 1;
    }
    return rightElement;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data.at(n1);
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return data.at(n2);
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return (n2 - n1 == -1);
}
