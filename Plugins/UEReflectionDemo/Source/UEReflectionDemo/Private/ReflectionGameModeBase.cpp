// Copyright Epic Games, Inc. All Rights Reserved.


#include "ReflectionGameModeBase.h"
#include "Student.h"
#include "MiddleStudent.h"
#include "GoodMiddleStudent.h"

AReflectionGameModeBase::AReflectionGameModeBase(const FObjectInitializer& ObjectInitializer)
	:AGameModeBase(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello,Reflection!"));

	UE_LOG(LogTemp, Log, TEXT("----------------------------------------------"));

	UStudent* Student = NewObject<UStudent>();
	UClass* StudentClass = Student->GetClass();
	//获取类名称
	FString ClassName = StudentClass->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Student's classname is %s."),*ClassName);
	//获取类标记
	bool bHasMinimalAPI = StudentClass->HasAnyClassFlags(EClassFlags::CLASS_MinimalAPI);
	if (bHasMinimalAPI)
		UE_LOG(LogTemp, Warning, TEXT("Student's classname is %s, flags has MinimalAPI"), *ClassName);

	for (FProperty* Property = StudentClass->PropertyLink; Property; Property = Property->PropertyLinkNext)
	{
		//获取属性名称
		FString PropertyName = Property->GetName();
		//获取属性类型
		FString PropertyType = Property->GetCPPType();
		if (PropertyType == "FString")
		{
			FStrProperty* StringProperty = CastField<FStrProperty>(Property);
			void* Addr = StringProperty->ContainerPtrToValuePtr<void>(Student);
			FString PropertyValue = StringProperty->GetPropertyValue(Addr);
			//获取属性元数据
			FString CategoryName = StringProperty->GetMetaData(TEXT("Category"));
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Student's properties has %s,Type is %s,Value is %s, Meta has %s."),*PropertyName, *PropertyType, *PropertyValue, *CategoryName);

			StringProperty->SetPropertyValue(Addr,"SettingValue");
			FString PropertyValueAfterSetting = StringProperty->GetPropertyValue(Addr);
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Student's properties has %s,Type is %s,Value after setting is %s."), *PropertyName, *PropertyType, *PropertyValueAfterSetting);

		}

	}

	UE_LOG(LogTemp, Log, TEXT("----------------------------------------------"));

	for (TFieldIterator<UFunction> IteratorOfFunction(StudentClass); IteratorOfFunction; ++IteratorOfFunction)
	{
		UFunction* Function = *IteratorOfFunction;
		//获取方法名称
		FString FunctionName = Function->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Student's functions has %s"), *FunctionName);
		//获取方法标记
		EFunctionFlags FunctionFlags = Function->FunctionFlags;
		UE_LOG(LogTemp, Warning, TEXT("Student's functions has %s, with flags: %x"), *FunctionName, FunctionFlags);

		for (TFieldIterator<FProperty> IteratorOfParam(Function); IteratorOfParam; ++IteratorOfParam)
		{
			FProperty* Param = *IteratorOfParam;
			FString ParamName = Param->GetName();
			FString ParamType = Param->GetCPPType();
			EPropertyFlags ParamFlags =  Param->GetPropertyFlags();
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Student's functions has %s,ParmType is %s,ParamName is %s, ParamFlag is %x"), 
				* FunctionName,
				* ParamType, 
				* ParamName,
				ParamFlags);

			UE_LOG(LogTemp, Log, TEXT("------------------------------"));
		}
	}

	//获取父类的名称
	UMiddleStudent* MiddleStudent = NewObject<UMiddleStudent>();
	UClass* ParentClass = MiddleStudent->GetClass()->GetSuperClass();
	FString ParentClassName = ParentClass->GetName();
	UE_LOG(LogTemp, Warning, TEXT("MiddleStudent's supercalssname is %s"), *ParentClassName);

	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	//判断一个类是另一个类的孩子类

	UClass* Class1 = UMiddleStudent::StaticClass();
	UClass* Class2 = UStudent::StaticClass();
	UClass* Class3 = AReflectionGameModeBase::StaticClass();

	if (Class1->IsChildOf(Class2))
	{
		UE_LOG(LogTemp, Warning, TEXT("Class1 is Class2's child."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Class1 is not Class2's child."));
	}
	if (Class3->IsChildOf(Class2))
	{
		UE_LOG(LogTemp, Warning, TEXT("Class3 is Class2's child."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Class3 is not Class2's child."));
	}

	//查找特定类的所有子类
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));

	TArray<UClass*> ClassResults;
	GetDerivedClasses(UStudent::StaticClass(), ClassResults, true);
	for (int32 Index = 0; Index < ClassResults.Num(); Index++)
	{
		UClass* ClassResult = ClassResults[Index];
		FString ClassResultName = ClassResult->GetName();
		UE_LOG(LogTemp, Warning, TEXT("UStudent::StaticClass 's derivedclass has %s."),*ClassResultName);
	}

	//查找由特定类生成的对象
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));

	UGoodMiddleStudent* GMS = NewObject<UGoodMiddleStudent>(this, FName("GSM1"));

	TArray<UObject*> ObjectResults;
	GetObjectsOfClass(UGoodMiddleStudent::StaticClass(), ObjectResults, false);
	for (UObject* ObjectResult : ObjectResults)
	{
		FString ObjectResultName = ObjectResult->GetName();
		UE_LOG(LogTemp, Warning, TEXT("UGoodMiddleStudent::StaticClass 's object has %s"), *ObjectResultName);
	}

	//根据字符串查找相应的类
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	UClass* FindedClass = FindObject<UClass>(ANY_PACKAGE, *FString("MiddleStudent"), true);
	if (FindedClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Class with name:UMiddleStudent has finded"));
	}

	//根据字符串查找枚举
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	UEnum* FindedEnum = FindObject<UEnum>(ANY_PACKAGE, *FString("EnumName"), true);
	if (FindedEnum)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enum with name:EnumName has finded"));

		//遍历枚举项名称
		for (int32 Index = 0; Index < FindedEnum->NumEnums(); Index++)
		{
			FString FindedEnumItemName = FindedEnum->GetNameStringByIndex(Index);
			UE_LOG(LogTemp, Warning, TEXT("Enum with name:EnumName has item : %s"),*FindedEnumItemName);
		}
	}

	//通过名称查找蓝图类
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));

	UBlueprint* FindedBlueprint = FindObject<UBlueprint>(ANY_PACKAGE, *FString("BP_BlueprintClass"));
	if (FindedBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blueprint with name:BP_BlueprintClass has finded"));
		//判断该类是蓝图类，还是Native类
		if (FindedBlueprint->IsNative())
		{
			UE_LOG(LogTemp, Warning, TEXT("BP_BlueprintClass is native class"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BP_BlueprintClass is blueprint class"));
		}
	}
	if (UStudent::StaticClass()->IsNative())
	{
		UE_LOG(LogTemp, Warning, TEXT("UStudent::StaticClass is native class"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UStudent::StaticClass is blueprint class"));
	}

	//遍历所有类
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	FString AllClassNames = "";
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		FString InterClassName = ClassIt->GetName();
		AllClassNames += InterClassName;
	}
	//UE_LOG(LogTemp, Warning, TEXT("AllClassName:%s"),*AllClassNames);


	//根据名称获取类方法
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	UGoodMiddleStudent* GoodMiddleStudent = NewObject<UGoodMiddleStudent>();
	UClass* GoodMiddleStudentClass = GoodMiddleStudent->GetClass();
	if (GoodMiddleStudentClass)
	{
		UFunction* PlayFunction = GoodMiddleStudentClass->FindFunctionByName(TEXT("Play"), EIncludeSuperFlag::ExcludeSuper);
		if (PlayFunction)
		{
			FString PlayFunctionName = PlayFunction->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Function with name:%s has finded in GoodMiddleStudentClass"),*PlayFunctionName);

			//通过反射调用类方法
			//1.给所有方法参数分配空间并初始化为0
			uint8* AllFunctionParam = static_cast<uint8*>(FMemory_Alloca(PlayFunction->ParmsSize));
			FMemory::Memzero(AllFunctionParam, PlayFunction->ParmsSize);

			//2.给所有方法参数赋值
			for (TFieldIterator<FProperty> IteratorOfFunctionParam(PlayFunction); IteratorOfFunctionParam; ++IteratorOfFunctionParam)
			{
				FProperty* FunctionParam = *IteratorOfFunctionParam;
				FString FunctionParamName = FunctionParam->GetName();
				if (FunctionParamName == FString("InGame"))
				{
					*FunctionParam->ContainerPtrToValuePtr<FString>(AllFunctionParam) = "Ball";
				}
			}

			//3.调用方法
			GoodMiddleStudent->ProcessEvent(PlayFunction, AllFunctionParam);
		}

		//Function.invoke
		UFunction* GoHomeFunction = GoodMiddleStudentClass->FindFunctionByName(TEXT("GoHome"), EIncludeSuperFlag::ExcludeSuper);
		if (GoHomeFunction)
		{
			//1.给所有方法参数分配空间并初始化为0
			uint8* AllFunctionParam = static_cast<uint8*>(FMemory_Alloca(GoHomeFunction->ParmsSize));
			FMemory::Memzero(AllFunctionParam, GoHomeFunction->ParmsSize);

			//2.创建FFrame
			FFrame Frame(nullptr, GoHomeFunction, &AllFunctionParam);

			//3.调用Invoke
			GoHomeFunction->Invoke(GoodMiddleStudent, Frame, &AllFunctionParam + GoHomeFunction->ReturnValueOffset);

			//4.获取返回值
			int* ResultValue = (int*)(&AllFunctionParam + GoHomeFunction->ReturnValueOffset);
			UE_LOG(LogTemp, Warning, TEXT("GoHome function's return value is : %d"), *ResultValue);
		}
	}

}