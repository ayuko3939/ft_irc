/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmmandUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohasega <yohasega@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:51:11 by yohasega          #+#    #+#             */
/*   Updated: 2025/03/27 18:23:23 by yohasega         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

std::vector<std::string> splitMessage(std::string message)
{
	std::istringstream iss(message);
	std::vector<std::string> words;
	std::string word;

	while (iss >> word)
	{
		trim(word);
		words.push_back(word);
	}
	return words;
}

// std::vector<std::string> splitByComma(const std::string &str)
// {
// 	std::vector<std::string> result;
// 	std::istringstream iss(str);
// 	std::string token;

// 	while (std::getline(iss, token, ','))
// 	{
// 		trim(token);
// 		if (!token.empty())
// 			result.push_back(token);
// 	}
// 	return result;
// }
