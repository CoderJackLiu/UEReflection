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
	//��ȡ������
	FString ClassName = StudentClass->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Student's classname is %s."),*ClassName);
	//��ȡ����
	bool bHasMinimalAPI = StudentClass->HasAnyClassFlags(EClassFlags::CLASS_MinimalAPI);
	if (bHasMinimalAPI)
		UE_LOG(LogTemp, Warning, TEXT("Student's classname is %s, flags has MinimalAPI"), *ClassName);

	for (FProperty* Property = StudentClass->PropertyLink; Property; Property = Property->PropertyLinkNext)
	{
		//��ȡ��������
		FString PropertyName = Property->GetName();
		//��ȡ��������
		FString PropertyType = Property->GetCPPType();
		if (PropertyType == "FString")
		{
			FStrProperty* StringProperty = CastField<FStrProperty>(Property);
			void* Addr = StringProperty->ContainerPtrToValuePtr<void>(Student);
			FString PropertyValue = StringProperty->GetPropertyValue(Addr);
			//��ȡ����Ԫ����
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
		//��ȡ��������
		FString FunctionName = Function->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Student's functions has %s"), *FunctionName);
		//��ȡ�������
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

	//��ȡ���������
	UMiddleStudent* MiddleStudent = NewObject<UMiddleStudent>();
	UClass* ParentClass = MiddleStudent->GetClass()->GetSuperClass();
	FString ParentClassName = ParentClass->GetName();
	UE_LOG(LogTemp, Warning, TEXT("MiddleStudent's supercalssname is %s"), *ParentClassName);

	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	//�ж�һ��������һ����ĺ�����

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

	//�����ض������������
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));

	TArray<UClass*> ClassResults;
	GetDerivedClasses(UStudent::StaticClass(), ClassResults, true);
	for (int32 Index = 0; Index < ClassResults.Num(); Index++)
	{
		UClass* ClassResult = ClassResults[Index];
		FString ClassResultName = ClassResult->GetName();
		UE_LOG(LogTemp, Warning, TEXT("UStudent::StaticClass 's derivedclass has %s."),*ClassResultName);
	}

	//�������ض������ɵĶ���
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));

	UGoodMiddleStudent* GMS = NewObject<UGoodMiddleStudent>(this, FName("GSM1"));

	TArray<UObject*> ObjectResults;
	GetObjectsOfClass(UGoodMiddleStudent::StaticClass(), ObjectResults, false);
	for (UObject* ObjectResult : ObjectResults)
	{
		FString ObjectResultName = ObjectResult->GetName();
		UE_LOG(LogTemp, Warning, TEXT("UGoodMiddleStudent::StaticClass 's object has %s"), *ObjectResultName);
	}

	//�����ַ���������Ӧ����
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	UClass* FindedClass = FindObject<UClass>(ANY_PACKAGE, *FString("MiddleStudent"), true);
	if (FindedClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Class with name:UMiddleStudent has finded"));
	}

	//�����ַ�������ö��
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	UEnum* FindedEnum = FindObject<UEnum>(ANY_PACKAGE, *FString("EnumName"), true);
	if (FindedEnum)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enum with name:EnumName has finded"));

		//����ö��������
		for (int32 Index = 0; Index < FindedEnum->NumEnums(); Index++)
		{
			FString FindedEnumItemName = FindedEnum->GetNameStringByIndex(Index);
			UE_LOG(LogTemp, Warning, TEXT("Enum with name:EnumName has item : %s"),*FindedEnumItemName);
		}
	}

	//ͨ�����Ʋ�����ͼ��
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));

	UBlueprint* FindedBlueprint = FindObject<UBlueprint>(ANY_PACKAGE, *FString("BP_BlueprintClass"));
	if (FindedBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blueprint with name:BP_BlueprintClass has finded"));
		//�жϸ�������ͼ�࣬����Native��
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

	//����������
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	FString AllClassNames = "";
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		FString InterClassName = ClassIt->GetName();
		AllClassNames += InterClassName;
	}
	//UE_LOG(LogTemp, Warning, TEXT("AllClassName:%s"),*AllClassNames);


	//�������ƻ�ȡ�෽��
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

			//ͨ����������෽��
			//1.�����з�����������ռ䲢��ʼ��Ϊ0
			uint8* AllFunctionParam = static_cast<uint8*>(FMemory_Alloca(PlayFunction->ParmsSize));
			FMemory::Memzero(AllFunctionParam, PlayFunction->ParmsSize);

			//2.�����з���������ֵ
			for (TFieldIterator<FProperty> IteratorOfFunctionParam(PlayFunction); IteratorOfFunctionParam; ++IteratorOfFunctionParam)
			{
				FProperty* FunctionParam = *IteratorOfFunctionParam;
				FString FunctionParamName = FunctionParam->GetName();
				if (FunctionParamName == FString("InGame"))
				{
					*FunctionParam->ContainerPtrToValuePtr<FString>(AllFunctionParam) = "Ball";
				}
			}

			//3.���÷���
			GoodMiddleStudent->ProcessEvent(PlayFunction, AllFunctionParam);
		}

		//Function.invoke
		UFunction* GoHomeFunction = GoodMiddleStudentClass->FindFunctionByName(TEXT("GoHome"), EIncludeSuperFlag::ExcludeSuper);
		if (GoHomeFunction)
		{
			//1.�����з�����������ռ䲢��ʼ��Ϊ0
			uint8* AllFunctionParam = static_cast<uint8*>(FMemory_Alloca(GoHomeFunction->ParmsSize));
			FMemory::Memzero(AllFunctionParam, GoHomeFunction->ParmsSize);

			//2.����FFrame
			FFrame Frame(nullptr, GoHomeFunction, &AllFunctionParam);

			//3.����Invoke
			GoHomeFunction->Invoke(GoodMiddleStudent, Frame, &AllFunctionParam + GoHomeFunction->ReturnValueOffset);

			//4.��ȡ����ֵ
			int* ResultValue = (int*)(&AllFunctionParam + GoHomeFunction->ReturnValueOffset);
			UE_LOG(LogTemp, Warning, TEXT("GoHome function's return value is : %d"), *ResultValue);
		}
	}

}