#include "ILS.h"
#include <iostream>
#include <optional>

ILS::ILS(Graph &graph, Propagate &evaluator, int iterations, float strength, int iterLimit)
	: graph(graph), evaluator(evaluator), maxIterations(iterations), iterLimit(iterLimit), perturbationStrength(strength)
{
	neighborhoods.push_back(LocalSearch::removeFix);
	neighborhoods.push_back(LocalSearch::addRemove);
}

int ILS::solutionSize(const std::vector<bool> &solution) const
{
	int count = 0;
	for (bool v : solution) {
		if (v) ++count;
	}
	return count;
}

std::vector<bool> ILS::runVND(const std::vector<bool> &start)
{
	std::vector<bool> current = start;
	bool improved = true;

	while (improved) {
		improved = false;
		for (auto &nb : neighborhoods) {
			std::vector<bool> candidate = nb(graph, evaluator, current);
			if (evaluator.isSolution(candidate) && solutionSize(candidate) < solutionSize(current)) {
				current = candidate;
				improved = true;
				break; // restart neighborhoods after improvement
			}
		}
	}
	return current;
}

std::vector<bool> ILS::run()
{
	std::optional<std::vector<bool>> none = std::nullopt;
	currentSolution = LocalSearch::Guloso(graph.getN(), 0.4f, graph, evaluator, none);
	bestSolution = currentSolution;
	bestOfAll = currentSolution;

	int iterWithoutImprove = 0;

	for (int i = 0; i < maxIterations; ++i) {
		std::vector<bool> shaken = LocalSearch::shake(graph, evaluator, currentSolution, perturbationStrength);
		std::vector<bool> improved = runVND(shaken);
		std::cout << getBestCost() << std::endl;

		int improvedSize = solutionSize(improved);
		int bestSize = solutionSize(bestSolution);

		if (evaluator.isSolution(improved) && improvedSize <= bestSize) {
			bestSolution = improved;
			iterWithoutImprove = 0;

			if (improvedSize < solutionSize(bestOfAll)) {
				bestOfAll = bestSolution;
				std::cout << improvedSize << std::endl;
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

const std::vector<bool> &ILS::getBestSolution() const
{
	return bestOfAll;
}

int ILS::getBestCost() const
{
	return solutionSize(bestOfAll);
}
