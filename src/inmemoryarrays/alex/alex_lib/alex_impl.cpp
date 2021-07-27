#include "alex_base.h"
#include "alex_nodes.h"
#include "alex_fanout_tree.h"

/*** Cost model weights ***/

using namespace alex;
using namespace alex::fanout_tree;

// Intra-node cost weights
double kExpSearchIterationsWeight = 20;
double kShiftsWeight = 0.5;

// TraverseToLeaf cost weights
double kNodeLookupsWeight = 20;
double kModelSizeWeight = 5e-7;

// https://en.wikipedia.org/wiki/CPUID#EAX=7,_ECX=0:_Extended_Features
bool cpu_supports_bmi() {
  return static_cast<bool>(alex::CPUID(7, 0).EBX() & (1 << 3));
}

// Collect all used fanout tree nodes and sort them
void alex::fanout_tree::collect_used_nodes(const std::vector<std::vector<FTNode>>& fanout_tree,
                        int max_level,
                        std::vector<FTNode>& used_fanout_tree_nodes) {
  max_level = std::min(max_level, static_cast<int>(fanout_tree.size()) - 1);
  for (int i = 0; i <= max_level; i++) {
    auto& level = fanout_tree[i];
    for (const FTNode& tree_node : level) {
      if (tree_node.use) {
        used_fanout_tree_nodes.push_back(tree_node);
      }
    }
  }
  std::sort(used_fanout_tree_nodes.begin(), used_fanout_tree_nodes.end(),
            [&](FTNode& left, FTNode& right) {
              // this is better than comparing boundary locations
              return (left.node_id << (max_level - left.level)) <
                  (right.node_id << (max_level - right.level));
            });
}