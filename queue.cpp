/**
 * @file queue.cpp
 * Implementation of the Queue class.
 *
 */
//treat the right hand side as the back of the queue
/**
 * Adds the parameter object to the back of the Queue.
 *
 * @param newItem object to be added to the Queue.
 */
template <class T>
void Queue<T>::enqueue(T newItem)
{
    myQueue.pushR(newItem);
}

/**
 * Removes the object at the front of the Queue, and returns it to the
 * caller.
 *
 * @return The item that used to be at the front of the Queue.
 */
template <class T>
T Queue<T>::dequeue()
{
    return myQueue.popL();
}

/**
 * Finds the object at the front of the Queue, and returns it to the
 * caller. Unlike dequeue(), this operation does not alter the queue.
 *
 * @return The item at the front of the queue.
 */
template <class T>
T Queue<T>::peek()
{
    return myQueue.peekL();
}

/**
 * Determines if the Queue is empty.
 *
 * @return bool which is true if the Queue is empty, false otherwise.
 */
template <class T>
bool Queue<T>::isEmpty() const
{
    return myQueue.isEmpty();
}
