#include <stdlib.h>
#include "librune.h"

const rn_LinsNode rn_DefaultLinsNode = {0, 0, 0};
const rn_LinsBody rn_DefaultLinsBody = {0, 0};

rn_LinsNode* rn_linsNewNode(rn_mem value)
{
    rn_LinsNode* node = (rn_LinsNode*) malloc(sizeof(rn_LinsNode));
	*node = rn_DefaultLinsNode;
	node->value = value;
	return node;
}

rn_lins rn_linsNew()
{
	rn_lins lins = (rn_lins) malloc(sizeof(rn_LinsBody));
	*lins = rn_DefaultLinsBody;
	return lins;
}

rn_uint rn_linsSize(rn_lins lins)
{
	rn_uint length = 1;
    rn_LinsNode* node;
    rn_LinsBody body = *lins;

	if (!body.first || !body.last) return (rn_sint) 0;

	node = body.first;

	while (rn_true)
	{
		if (!node->next) break;
		node = node->next;
		if (length == rn_UINT_MAX) break;
		length++;
	}
	return length;
}

void rn_linsPush(rn_lins lins, rn_mem value)
{
    rn_LinsNode* node = rn_linsNewNode(value);

	if (!lins->first || !lins->last)
	{
		lins->first = node;
		lins->last = node;
		return;
	}

	lins->last->next = node;
	lins->last = node;
}

rn_mem rn_linsPop(rn_lins lins)
{
    rn_LinsNode* node;
	rn_mem value;

	if (!lins->first || !lins->last) return rn_null;

	node = lins->last;

	if (lins->first == node)
	{
		lins->first = rn_null;
		lins->last = rn_null;
		value = node->value;
		free(node);
		return value;
	}

	node->prev->next = rn_null;
	lins->last = node->prev;
	value = node->value;
	free(node);
	return value;
}

rn_mem rn_linsPull(rn_lins lins)
{
    rn_LinsNode* node;
	rn_mem value;

	if (!lins->first || !lins->last) return rn_null;

	node = lins->first;

	if (lins->last == node)
	{
		lins->first = rn_null;
		lins->last = rn_null;
		value = node->value;
		free(node);
		return value;
	}

	node->next->prev = rn_null;
	lins->first = node->next;
	value = node->value;
	free(node);
	return value;
}

rn_mem rn_linsGet(rn_lins lins, rn_sint index)
{
	rn_sint i;
    rn_LinsNode* node;
	rn_mem value;

	if (!lins->first || !lins->last) return rn_null;

	if (index < 0)
	{
		node = lins->last;
		i = -1;

		while (rn_true)
		{
			if (i <= index) break;
			if (!node->prev) return rn_null;
			i--;
			node = node->prev;
		}

		return node->value;
	}

	node = lins->first;
	i = 0;

	while (rn_true)
	{
		if (i >= index) break;
		if (!node->next) return rn_null;
		i++;
		node = node->next;
	}

	return node->value;
}

void rn_linsSet(rn_lins lins, rn_sint index, rn_mem value)
{
	rn_sint i;
    rn_LinsNode* node;
    rn_LinsNode* newNode;

	if (index == -1) return rn_linsPush(lins, value);

	newNode = rn_linsNewNode(value);

	if (!lins->first || !lins->last)
	{
		lins->first = newNode;
		lins->last = newNode;
		if (!index) return;
		return;
	}

	if (!index)
	{
		newNode->next = lins->first;
		lins->first->prev = newNode;
		lins->first = newNode;
		return;
	}

	if (index < -1)
	{
		node = lins->last;
		i = -2;

		while (rn_true)
		{
			if (!node->prev)
			{
				newNode->next = node;
				lins->first = newNode;
				return;
			}
			if (i <= index) break;
			node = node->prev;
			i--;
		}

		newNode->next = node;
		newNode->prev = node->prev;
		node->prev->next = newNode;
		node->prev = newNode;
		return;
	}

	node = lins->first;
	i = 1;

	while (rn_true)
	{
		if (!node->next)
		{
			newNode->prev = node;
			lins->last = newNode;
			return;
		}
		if (i <= index) break;
		node = node->next;
		i++;
	}

	newNode->prev = node;
	newNode->next = node->next;
	node->next->prev = newNode;
	node->next = newNode;
	return;
}

void rn_linsDeleteIndex(rn_lins lins, rn_sint index)
{
	rn_sint i;
    rn_LinsNode* node;

	if (!lins->first || !lins->last) return;

	if (!index)
	{
		free(rn_linsPull(lins));
		return;
	}

	if (index == -1)
	{
		free(rn_linsPop(lins));
		return;
	}

	if (index < 0)
	{
		node = lins->last;
		i = -1;

		while (rn_true)
		{
			if (!node->prev)
			{
				lins->first = node->next;
				node->next->prev = rn_null;
				free(node->value);
				free(node);
				return;
			}
			if (i <= index) break;
			node = node->prev;
			i--;
		}

		node->next->prev = node->prev;
		node->prev->next = node->next;
		free(node->value);
		free(node);
		return;
	}

	node = lins->first;
	i = 0;

	while (rn_true)
	{
		if (!node->next)
		{
			lins->last = node->prev;
			node->prev->next = rn_null;
			free(node->value);
			free(node);
			return;
		}
		if (i >= index) break;
		node = node->next;
		i++;
	}

	node->next->prev = node->prev;
	node->prev->next = node->next;
	free(node->value);
	free(node);
	return;
}

void rn_linsDelete(rn_lins lins, rn_mem value)
{
    rn_LinsNode* node = lins->first;

	free(value);

	if (lins->last == node && node->value == value)
	{
		lins->first = rn_null;
		lins->last = rn_null;
		free(node);
		return;
	}

	while (rn_true)
	{
		if (!node) return;
		if (node->value == value)
		{
			if (node == lins->first)
			{
				lins->first = node->next;
				node->next->prev = rn_null;
				free(node);
				return;
			}

			if (node == lins->last)
			{
				lins->last = node->prev;
				node->prev->next = rn_null;
				free(node);
				return;
			}

			node->prev->next = node->next;
			node->next->prev = node->prev;
			free(node);
			return;
		}
		node = node->next;
	}
}

void rn_linsRelease(rn_lins lins)
{
    rn_LinsNode* node;
    rn_LinsNode* nodeNext;

	if (!lins->first || !lins->last)
	{
		free(lins);
		return;
	}

	node = lins->first;

	while (rn_true)
	{
		free(node->value);
		nodeNext = node->next;
		free(node);
		if (!nodeNext) return;
		node = nodeNext;
	}
}

void rn_linsForEach(rn_lins lins, rn_LinsForEachHandler forEachHandler, rn_mem data)
{
	rn_uint count = 0;
    rn_LinsNode* node = lins->first;
    rn_LinsForEachHandler handler = forEachHandler;

	while (rn_true)
	{
		if (!node) return;
		handler(count++, node->value, data);
		node = node->next;
	}
}
