// CountSubsetOrderedCollection
// 
// This program does the following.
// 
// (1) A user sets integer variables [n,k] and chooses the
//     lexicographic min-min or the lexicographic min-max.
// (2) A set X = {1,2,...,n} is constructed.
// (3) The collection of subsets of X of size k is ordered
//     according to the order chosen in (1).
// (4) For each t=1,2,3,..., C is constructed as the first
//     t-many sets in the collection in (3).
// (5) Compute the number of sets of size k-1 that are subsets
//     of at least one member of C. 
// (6) Report the number in (5) for each t=1,2,3,...

#include <iostream>
#include <cstdlib>
#include <set>
#include <vector>
#include <algorithm> // std::include

void print_set(std::set<int> S);
void print_vector(std::vector<std::set<int>> V);
std::set<std::set<int>> power_set(std::set<int> S);
std::vector<std::set<std::set<int>>> sort_collection(std::set<std::set<int>> C, int n);
int count_subset(int k, std::set<std::set<int>> C, std::vector<std::set<std::set<int>>> P_sorted);
std::vector<std::set<int>> order_Pk(int k, std::vector<std::set<std::set<int>>> P_sorted);
bool is_prior(std::set<int> A, std::set<int> B);
bool alt_is_prior(std::set<int> A, std::set<int> B);

int main() {

  std::set<int> X;
  int n, k;

  // Constructing X

  std::cout << "Set the size of X [n]: ";
  std::cin >> n;
  std::cout << "\n";

  X.clear();
  for (int i = 0; i < n; i++)
  {
    X.insert(i + 1);
  }

  std::cout << "The set X is constructed as:\n X = ";
  print_set(X);
  std::cout << "\n\n";

  // Constructing the power set of X

  std::cout << "Generating the power set of X...\n";
  std::set<std::set<int>> P = power_set(X);
  std::cout << "The construction of the power set of X is complete. It contains " << P.size() << " subsets of X.\n\n";

  // Sorting the power set of X

  std::cout << "Sorting the power set of X by size...\n";
  std::vector<std::set<std::set<int>>> P_sorted = sort_collection(P, n);
  std::cout << "Sorting of the power set of X is complete.\n\n";

  std::cout << "The number of sets in each layer:";
  for (int i = 0; i < n + 1; ++i)
  {
    std::cout << ' ' << P_sorted[i].size();
  }
  std::cout << "\n\n";

  // Set k

  std::cout << "Set the size of subsets of X [k]: ";
  std::cin >> k;
  std::cout << "\n";

  std::cout << "There are " << P_sorted[k].size()
    << " subsets of X that contains k=" << k << " elements.\n\n";

  // Order the collection of sets of size k
  
  std::vector<std::set<int>> V = order_Pk(k, P_sorted);

  std::cout << "The subsets of X of size k are ordered as follows.\n\n";
  print_vector(V);
  std::cout << "\n\n";

  // Count subsets for each proceeding sets in ordered collection

  std::set<std::set<int>> C;
  C.clear();

  std::cout << "RESULT:\n\n";
  std::cout << "t\t|\t\#\n";

  for (auto it_V = V.begin(); it_V != V.end(); ++it_V)
  {
    C.insert(*it_V);
    int num = count_subset(k, C, P_sorted);
    std::cout << C.size() << "\t|\t" << num << "\n";
  }

  std::cout << "\n";

  std::system("pause");

  return 0;

}

void print_set(std::set<int> S) {

  std::cout << "{ ";

  for (auto it = S.begin(); it != S.end(); ++it)
  {
    if (it != S.begin()) std::cout << ", ";
    std::cout << *it;
  }

  std::cout << " }";

}

void print_vector(std::vector<std::set<int>> V) {

  std::cout << "{\n";

  for (auto it_V = V.begin(); it_V != V.end(); ++it_V)
  {
    std::cout << "    ";
    print_set(*it_V);
    std::cout << "\n";
  }

  std::cout << "}";

}

std::set<std::set<int>> power_set(std::set<int> S) {

  std::set<std::set<int>> P;
  std::set<int> empty;
  P.clear();
  empty.clear();
  P.insert(empty);

  for (auto s = S.begin(); s != S.end(); ++s)
  {
    for (auto p = P.begin(); p != P.end(); ++p)
    {
      std::set<int> union_ps = *p;
      union_ps.insert(*s);
      P.insert(union_ps);
    }
  }

  return P;

}

std::vector<std::set<std::set<int>>> sort_collection(std::set<std::set<int>> C, int n) {

  std::vector<std::set<std::set<int>>> C_sorted;
  std::set<std::set<int>> empty;
  empty.clear();
  C_sorted.assign(n + 1, empty);

  for (auto it_C = C.begin(); it_C != C.end(); ++it_C)
  {
    C_sorted[it_C->size()].insert(*it_C);
  }

  return C_sorted;

}

int count_subset(int k, std::set<std::set<int>> C, std::vector<std::set<std::set<int>>> P_sorted) {

  if (k < 1) return 0;

  int num = 0;
  for (auto it_P = P_sorted[k - 1].begin(); it_P != P_sorted[k - 1].end(); ++it_P)
  {
    for (auto it_C = C.begin(); it_C != C.end(); ++it_C)
    {
      if (std::includes(it_C->begin(), it_C->end(), it_P->begin(), it_P->end()))
      {
        ++num;
        break;
      }
    }
  }

  return num;

}

std::vector<std::set<int>> order_Pk(int k, std::vector<std::set<std::set<int>>> P_sorted) {

  std::vector<std::set<int>> V;

  bool (*compare)(std::set<int> A, std::set<int> B);
  int ans;
  std::cout << "Type any-nonzero integer to use the lex min-min or 0 to use the lex min-max: ";
  std::cin >> ans;
  std::cout << "\n";
  if (ans)
  {
    compare = alt_is_prior;
  }
  else
  {
    compare = is_prior;
  }

  for (auto it_P = P_sorted[k].begin(); it_P != P_sorted[k].end(); ++it_P)
  {
    if (V.empty())
    {
      V.push_back(*it_P);
      continue;
    }

    int flag = 1;
    for (auto it_V = V.begin(); it_V != V.end(); ++it_V)
    {
      if (compare(*it_P, *it_V))
      {
        V.insert(it_V, *it_P);
        flag = 0;
        break;
      }
    }
    
    if (flag)
    {
      V.push_back(*it_P);
    }
  }

  return V;

}

bool is_prior(std::set<int> A, std::set<int> B) {

  auto it_A = A.rbegin();
  auto it_B = B.rbegin();
  
  while (it_A != A.rend() && it_B != B.rend())
  {
    if (*it_A < *it_B)
    {
      return true;
    }
    else if (*it_A > *it_B)
    {
      return false;
    }
    ++it_A;
    ++it_B;
  }

  return false;

}

bool alt_is_prior(std::set<int> A, std::set<int> B) {

  auto it_A = A.begin();
  auto it_B = B.begin();

  while (it_A != A.end() && it_B != B.end())
  {
    if (*it_A < *it_B)
    {
      return true;
    }
    else if (*it_A > *it_B)
    {
      return false;
    }
    ++it_A;
    ++it_B;
  }

  return false;

}
