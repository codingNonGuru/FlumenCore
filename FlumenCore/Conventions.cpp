#include <stdio.h>

#include <GL/glew.h>

#include "Conventions.hpp"

void* AllocateMemory(unsigned int size)
{
	return malloc(size);
}

void* CopyMemory(void *destination, const void *source, size_t length)
{
	return memcpy(destination, source, length);
}

int CompareMemory(const void *ptr1, const void *ptr2, size_t length)
{
	return memcmp(ptr1, ptr2, length);
}

int CompareStrings(const char *firstString, const char *secondString)
{
	return strcmp(firstString, secondString);
}

Length GetStringLength(const char* string)
{
	return strlen(string);
}

const char* FindString(const char* firstString, const char* secondString)
{
	return strstr(firstString, secondString);
}

const char* FindLastOccurrence(const char* string, char character)
{
	return strrchr(string, character);
}

void DebugGl(const char* file, int line)
{
	return;

	unsigned int numMsgs = 32;

	GLint maxMessageLength = 0;
	glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMessageLength);

	GLchar* msgData = malloc(sizeof(GLchar) * numMsgs * maxMessageLength);
	GLenum* sources = malloc(sizeof(GLenum) * numMsgs);
	GLenum* types = malloc(sizeof(GLenum) * numMsgs);
	GLenum* severities = malloc(sizeof(GLenum) * numMsgs);
	GLuint* ids = malloc(sizeof(GLuint) * numMsgs);
	GLsizei* lengths = malloc(sizeof(GLsizei) * numMsgs);

	GLuint numFound = glGetDebugMessageLog(numMsgs, numMsgs * maxMessageLength, sources, types, ids, severities, lengths, msgData);

	if(numFound != 0)
	{
		std::cout<<"OpenGL Debugger called from: "<<FindLastOccurrence(file, '/') + 1<<":"<<line<<"\n";
	}

	GLchar* message = msgData;
	for(int i = 0; i < numFound; ++i)
	{
		std::cout<<message<<"\n";
		message += strlen(message) + 1;
	}

	if(numFound != 0)
	{
		std::cout<<"\n";
	}

	free(msgData);
	free(sources);
	free(types);
	free(severities);
	free(ids);
	free(lengths);
}
