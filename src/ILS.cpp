#include "ILS.h"
#include <iostream>
#include <optional>
#include <cstdint>

ILS::ILS(Graph &graph, Propagate &evaluator, int iterations, float strength, int iterLimit)
	: graph(graph), evaluator(evaluator), maxIterations(iterations), iterLimit(iterLimit), perturbationStrength(strength)
{
	neighborhoods.push_back(LocalSearch::removeFix);
	neighborhoods.push_back(LocalSearch::addRemove);
}

int ILS::solutionSize(const std::vector<uint8_t> &solution) const
{
	int count = 0;
	for (uint8_t v : solution) {
		if (v) ++count;
	}
	return count;
}

std::vector<uint8_t> ILS::runVND(const std::vector<uint8_t> &start)
{
	std::vector<uint8_t> current = start;
	bool improved = true;

	while (improved) {
		improved = false;
		for (auto &nb : neighborhoods) {
			std::vector<uint8_t> candidate = nb(graph, evaluator, current);
			if (evaluator.isSolution(candidate) && solutionSize(candidate) < solutionSize(current)) {
				current = candidate;
				improved = true;
				break; // restart neighborhoods after improvement
			}
		}
	}
	return current;
}

std::vector<uint8_t> ILS::run()
{
	std::optional<std::vector<uint8_t>> none = std::nullopt;
	currentSolution = LocalSearch::Guloso(graph.getN(), 0.4f, graph, evaluator, none);
	bestSolution = currentSolution;
	bestOfAll = currentSolution;
	int iterWithoutImprove = 0;
	for (int i = 0; i < maxIterations; ++i) {
		std::vector<uint8_t> shaken = LocalSearch::shake(graph, evaluator, currentSolution, perturbationStrength);
		std::vector<uint8_t> improved = runVND(shaken);
		int improvedSize = solutionSize(improved);
		int bestSize = solutionSize(bestSolution);
		if (evaluator.isSolution(improved) && improvedSize <= bestSize) {
			bestSolution = improved;
			iterWithoutImprove = 0;
			if (improvedSize < solutionSize(bestOfAll)) {
				bestOfAll = bestSolution;
			}
		} else {
			++iterWithoutImprove;
		}
		if (evaluator.isSolution(improved)) {
			currentSolution = improved;
		}
		if (iterWithoutImprove > iterLimit) {
			currentSolution = LocalSearch::shake(graph, evaluator, currentSolution, 0.5f);
			bestSolution = currentSolution;
			iterWithoutImprove = 0;
		}
	}
	return bestOfAll;
}

const std::vector<uint8_t> &ILS::getBestSolution() const
{
	return bestOfAll;
}

int ILS::getBestCost() const
{
	return solutionSize(bestOfAll);
}