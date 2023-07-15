#include "il2cppresolver/Il2CppResolver.hpp"
#include "Component.hpp"
#include "globals.hpp"
#include "Methods.hpp"

using namespace Unity;

void ExceptionThrownLog()
{
	LoggerClass::Log("[ERROR] An exception occurred, did you set the right return and argument type?\n");
}

void OnUpdate()
{
	if (Globals::callMethod)
	{
		switch (Globals::MethodRetType_Current)
		{
			case RetType::Void:
			{
				switch (Globals::MethodArgType_Current)
				{
					case ArgType::noArg:
					{
						try {
							ComponentClass::Get()->CallMethodSafe<void*>(MethodArgs::MethodName.c_str());
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::inT:
					{
						try {
							ComponentClass::Get()->CallMethodSafe<void*, int>(MethodArgs::MethodName.c_str(), MethodArgs::MethodInt);
						}
						catch (...) 
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::floaT:
					{
						try {
							ComponentClass::Get()->CallMethodSafe<void*, float>(MethodArgs::MethodName.c_str(), MethodArgs::MethodFloat);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::booL:
					{
						try {
							ComponentClass::Get()->CallMethodSafe<void*, bool>(MethodArgs::MethodName.c_str(), MethodArgs::MethodBool);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::strinG:
					{
						try {
							ComponentClass::Get()->CallMethodSafe<void*, Unity::System_String*>(MethodArgs::MethodName.c_str(), IL2CPP::String::New(MethodArgs::MethodString));
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}
				}
				break;
			}

			case RetType::Int:
			{
				switch (Globals::MethodArgType_Current)
				{
					case ArgType::noArg:
					{
						try {
							MethodArgs::RetInt = ComponentClass::Get()->CallMethodSafe<int>(MethodArgs::MethodName.c_str());
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}
					case ArgType::inT:
					{
						try {
							MethodArgs::RetInt = ComponentClass::Get()->CallMethodSafe<int, int>(MethodArgs::MethodName.c_str(), MethodArgs::MethodInt);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::floaT:
					{
						try {
							MethodArgs::RetInt = ComponentClass::Get()->CallMethodSafe<int, float>(MethodArgs::MethodName.c_str(), MethodArgs::MethodFloat);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::booL:
					{
						try {
							MethodArgs::RetInt = ComponentClass::Get()->CallMethodSafe<int, bool>(MethodArgs::MethodName.c_str(), MethodArgs::MethodBool);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::strinG:
					{
						try {
							MethodArgs::RetInt = ComponentClass::Get()->CallMethodSafe<int, Unity::System_String*>(MethodArgs::MethodName.c_str(), IL2CPP::String::New(MethodArgs::MethodString));
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}
				}
				break;
			}

			case RetType::Float:
			{
				switch (Globals::MethodArgType_Current)
				{
					case ArgType::noArg:
					{
						try {
							MethodArgs::RetFloat = ComponentClass::Get()->CallMethodSafe<float>(MethodArgs::MethodName.c_str());
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::inT:
					{
						try {
							MethodArgs::RetFloat = ComponentClass::Get()->CallMethodSafe<float, int>(MethodArgs::MethodName.c_str(), MethodArgs::MethodInt);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::floaT:
					{
						try {
							MethodArgs::RetFloat = ComponentClass::Get()->CallMethodSafe<float, float>(MethodArgs::MethodName.c_str(), MethodArgs::MethodFloat);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::booL:
					{
						try {
							MethodArgs::RetFloat = ComponentClass::Get()->CallMethodSafe<float, bool>(MethodArgs::MethodName.c_str(), MethodArgs::MethodBool);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::strinG:
					{
						try {
							MethodArgs::RetFloat = ComponentClass::Get()->CallMethodSafe<float, Unity::System_String*>(MethodArgs::MethodName.c_str(), IL2CPP::String::New(MethodArgs::MethodString));
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}
				}
				break;
			}

			case RetType::Bool:
			{
				switch (Globals::MethodArgType_Current)
				{
					case ArgType::noArg:
					{
						try {
							MethodArgs::RetBool = ComponentClass::Get()->CallMethodSafe<bool>(MethodArgs::MethodName.c_str());
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::inT:
					{
						try {
							MethodArgs::RetBool = ComponentClass::Get()->CallMethodSafe<bool, int>(MethodArgs::MethodName.c_str(), MethodArgs::MethodInt);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::floaT:
					{
						try {
							MethodArgs::RetBool = ComponentClass::Get()->CallMethodSafe<bool, float>(MethodArgs::MethodName.c_str(), MethodArgs::MethodFloat);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::booL:
					{
						try {
							MethodArgs::RetBool = ComponentClass::Get()->CallMethodSafe<bool, bool>(MethodArgs::MethodName.c_str(), MethodArgs::MethodBool);
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}

					case ArgType::strinG:
					{
						try {
							MethodArgs::RetBool = ComponentClass::Get()->CallMethodSafe<bool, Unity::System_String*>(MethodArgs::MethodName.c_str(), IL2CPP::String::New(MethodArgs::MethodString));
						}
						catch (...)
						{
							ExceptionThrownLog();
						}
						break;
					}
				}
				break;
			}
		}
		Globals::callMethod = false;
	}
}
