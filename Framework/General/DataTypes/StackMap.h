//====================================================================================================================
//StackMap
//====================================================================================================================
#ifndef __STACKMAP_H
#define __STACKMAP_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#define STACK_MAX 100
//-----------------------------------------------------------------------------------------------------------------
template <typename KEY, typename DATA>
struct Node
{
	DATA data;
	KEY key;
};
//-----------------------------------------------------------------------------------------------------------------
template <typename KEY, typename DATA>
class StackMap
{
public:
	StackMap()
	{
		m_stack_count = 0;
	}

	StackMap(const StackMap& o)
	{
	}

	~StackMap()
	{
	}

	//Push a node onto the stack
	void push(KEY key, DATA data)
	{
		//Cannot add any more data onto the stack
		if (Full()) return;

		Node<KEY, DATA> new_node;

		new_node.key = key;
		new_node.data = data;

		m_head[m_stack_count] = new_node;

		m_stack_count++;
	}

	//Pop a node off the stack
	void pop(KEY& key, DATA& data)
	{
		//Cannot pop any more data off the stack
		if (Empty()) return;

		m_stack_count--;

		key = m_head[m_stack_count].key;
		data = m_head[m_stack_count].data;
	}

	int GetStackCount() { return m_stack_count; }

	void top(KEY& key, DATA& data)
	{
		//Cannot pop any more data off the stack
		if (Empty()) return;

		key = m_head[m_stack_count - 1].key;
		data = m_head[m_stack_count - 1].data;
	}

	void GetNodeAtIndex(KEY& key, DATA& data, int id)
	{
		//Cannot pop any more data off the stack
		if (Empty()) return;

		key = m_head[id].key;
		data = m_head[id].data;
	}

	void IncIndex()
	{
		m_stack_count++;
	}

	void Clear()
	{
		m_stack_count = 0;
	}

	bool Full()
	{
		return (m_stack_count >= STACK_MAX - 1);
	}

	bool Empty()
	{
		return (m_stack_count == 0);
	}

private:
	
	int m_stack_count;

	Node<KEY, DATA> m_head[STACK_MAX];
};
//-----------------------------------------------------------------------------------------------------------------
#endif//__STACKMAP_H