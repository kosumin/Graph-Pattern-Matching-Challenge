
#include "common.h"
#include "graph.h"

bool check(std::vector<Vertex> arr, Graph& data, Graph& query);

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: ./program <data graph file> <query graph file> "
                 "<result file>\n";
    return EXIT_FAILURE;
  }

  std::string data_file_name = argv[1];
  std::string query_file_name = argv[2];
  std::string result_file_name = argv[3];

  Graph data(data_file_name);
  Graph query(query_file_name, true);
  std::vector<std::vector<Vertex>> arr;
  std::ifstream fin(result_file_name);

  if (fin.is_open()) {
    char type;
    int num;
    int j = 0;
    Vertex id;

    fin >> type >> num;
    while(fin >> type) {
        if (type == 'a') {
            std::vector<Vertex> _arr;
            for (int i = 0; i < num ; i++) {
                fin >> id;
                _arr.push_back(id);
            }
            arr.push_back(_arr);
            j++;
        }
    }

  }
    fin.close();

    int correct = 0;
    int incorrect = 0;

    for (int i = 0; i < arr.size(); i++) {
        check(arr[i], data, query) ? correct++ : incorrect++;
    }

    std::cout << "correct: " << correct << " incorrect: " << incorrect << std::endl;
  return EXIT_SUCCESS;
}

bool check(std::vector<Vertex> arr, Graph& data, Graph& query) {
    int32_t num = query.GetNumVertices();
    int32_t j;
    for (int32_t i = 0; i < num; ++i) {
        for(j = i + 1; j < num; ++j) {
            if (query.IsNeighbor(i, j)) {
                if (!data.IsNeighbor(arr[i], arr[j])) return false;
            }
        }

    }
    return true;
}
