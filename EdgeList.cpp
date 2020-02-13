#include <iostream>
#include <vector>

struct IGraph
{
	virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};

class EdgeList : public IGraph
{
public:

	EdgeList(int count);
	EdgeList(const IGraph& graph);
	virtual ~EdgeList();

	virtual void AddEdge(int from, int to);

	virtual int VerticesCount() const;

	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const;
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const;

	void Print();

private:

	std::vector<std::vector<int>> adjacencyLists;
};

EdgeList::EdgeList(int count)
{
	adjacencyLists.resize(0);
}

EdgeList::EdgeList(const IGraph& graph)
{
	adjacencyLists.resize(graph.VerticesCount());

	std::vector<int> tmp(graph.VerticesCount(), 0);

	int g = 0;

	for (int i = 0; i < adjacencyLists.size(); ++i)
	{
		graph.GetNextVertices(i, tmp);
		for (int j = 0; j < tmp.size(); ++j)
		{
			adjacencyLists[g].push_back(i);
			adjacencyLists[g].push_back(tmp[j]);
			++g;
		}

	}
}

EdgeList::~EdgeList()
{
}

void EdgeList::AddEdge(int from, int to)
{
	adjacencyLists.resize(adjacencyLists.size() + 1);

	std::vector<int> tmp;

	adjacencyLists[adjacencyLists.size() - 1].push_back(from);
	adjacencyLists[adjacencyLists.size() - 1].push_back(to);
}

int EdgeList::VerticesCount() const
{
	int result = -1;

	for (int i = 0; i < adjacencyLists.size(); ++i)
		if (result < adjacencyLists[i][0])
			result = adjacencyLists[i][0];

	return result + 1;
}

void EdgeList::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
	vertices.clear();

	for (int i = 0; i < adjacencyLists.size(); ++i)
		if (adjacencyLists[i][0] == vertex)
			vertices.push_back(adjacencyLists[i][1]);
	}

void EdgeList::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
	vertices.clear();

	for (int i = 0; i < adjacencyLists.size(); ++i)
		if (adjacencyLists[i][1] == vertex)
			vertices.push_back(adjacencyLists[i][0]);
}

void EdgeList::Print()
{
	for (int i = 0; i < adjacencyLists.size(); ++i)
	{
		for (int j = 0; j < adjacencyLists[i].size(); ++j)
			std::cout << adjacencyLists[i][j] << " ";
		std::cout << std::endl;
	}
}

int main()
{
	EdgeList graph(7);
	graph.AddEdge(0, 1);
	graph.AddEdge(0, 5);
	graph.AddEdge(1, 2);
	graph.AddEdge(1, 3);
	graph.AddEdge(1, 5);
	graph.AddEdge(1, 6);
	graph.AddEdge(3, 2);
	graph.AddEdge(3, 6);
	graph.AddEdge(4, 3);
	graph.AddEdge(5, 4);
	graph.AddEdge(5, 6);
	graph.AddEdge(6, 4);

	graph.Print();

	std::vector<int> tmp(graph.VerticesCount(), 0);

	graph.GetNextVertices(1, tmp);

	for (int i = 0; i < tmp.size(); ++i)
		std::cout << tmp[i] << " ";
	std::cout << std::endl;

	graph.GetPrevVertices(6, tmp);

	for (int i = 0; i < tmp.size(); ++i)
		std::cout << tmp[i] << " ";

	system("pause");

	return 0;
}