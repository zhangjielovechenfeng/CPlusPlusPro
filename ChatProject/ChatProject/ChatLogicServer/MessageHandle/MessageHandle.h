#pragma once
class MessageHandle
{
public:
	MessageHandle();
	virtual ~MessageHandle();

public:
	virtual bool Handle() = 0;
};

