#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include <algorithm>

struct ImArea {
	ImVec2 min, max, size;

	ImArea(ImVec2 min = {}, ImVec2 max = {}) : min(min), max(max) {
		
		this->min.x = std::min(min.x, max.x);
		this->min.y = std::min(min.y, max.y);

		this->max.x = std::max(min.x, max.x);
		this->max.y = std::max(min.y, max.y);

		size = max - min;
	}

	ImVec2 Clamp(ImVec2 vec) {
		vec.x = std::clamp(vec.x, min.x, max.x);
		vec.y = std::clamp(vec.y, min.y, max.y);
		return vec;
	}

	ImArea Clamp(ImArea area) {
		area.min = Clamp(area.min);
		area.max = Clamp(area.max);
		return area;
	}

	ImVec2 GetCenter() {
		return (min + max) / 2;
	}

	ImArea Sub(ImVec2 start, ImVec2 end) {
		return Clamp(ImArea{ min + start, min + end });
	}

	ImArea SubSize(ImVec2 start, ImVec2 size) {
		return Sub(start, start + size);
	}

	ImArea RelSub(ImVec2 startRatio, ImVec2 endRatio) {
		return Sub(startRatio * size,  endRatio * size);
	}

	ImArea Column(int index, int amount) {
		float width = size.x / amount;
		return Sub(ImVec2{ index * width, 0 }, ImVec2{ (index + 1) * width, size.y });
	}

	ImArea Row(int index, int amount) {
		float height = size.y / amount;
		return Sub(ImVec2{ 0, index * height }, ImVec2{ size.x, (index + 1) * height });
	}

	ImArea Pad(ImVec2 amount) {
		return ImArea(min + amount, max - amount);
	}

	ImArea Pad(float amount) {
		return Pad({ amount, amount });
	}

	ImArea ShrinkToCenterSquare() {
		ImVec2 center = GetCenter();
		float halfSquareSize = std::min(size.x, size.y) / 2;
		ImVec2 halfSquareSizeVec = ImVec2(halfSquareSize, halfSquareSize);
		return ImArea(center - halfSquareSizeVec, center + halfSquareSizeVec);
	}
};