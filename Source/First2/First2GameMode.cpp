// Copyright Epic Games, Inc. All Rights Reserved.

#include "First2GameMode.h"

#include <ivehandler.h>
#include <ThirdParty/CryptoPP/5.6.5/include/smartptr.h>

#include "ComplexClas.h"
#include "FADTask.h"
#include "First2HUD.h"
#include "First2Character.h"
#include "FMDTask.h"
#include "RawClas.h"
#include "SimpleIntf.h"
#include "TheCppObject.h"
#include "X.h"
#include "XmlFile.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


// DEFINE_LOG_CATEGORY(TheGMLog) �� DECLARE_LOG_CATEGORY_EXTERN(TheGMLog, Log, All) ������ʹ�õ�
DEFINE_LOG_CATEGORY(TheGMLog);

AFirst2GameMode::AFirst2GameMode()
	: Super()
{
	////////////////////////////////////////////////////////////////////////// չʾ�˾�̬���صĴ���

	// ClassFinder���÷�1��û��blueprintǰ׺��.xx
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// ClassFinder���÷�2������blueprintǰ׺������ʹ�� X.X_C �ĸ�ʽȥ����
	static ConstructorHelpers::FClassFinder<APawn> C(TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter_C'"));
	DefaultPawnClass = C.Class;


	// StaticLoadObject ��̬���� ���������ʱû�����Ϊʲô����������˵��OK�ģ��������ڹ��캯����Ż�����ɡ�
	// [UE4]C++ʵ�ֶ�̬����UObject��StaticLoadObject()����Texture��MaterialΪ��-pudn.com https://www.pudn.com/news/628f8299bf399b7f351e34fc.html

	// �ҵ�Class�ĵ��������� 
	// static ConstructorHelpers::FObjectFinder<APawn> C2(TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter'"));
	//		auto justSee=	C2.Object->GetClass();
	// DefaultPawnClass = C2.Object->GetClass();

	// static ConstructorHelpers::FObjectFinder<APawn> C3(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter"));
	// auto justSee2 = C3.Object->GetClass();


	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString(""));


	UTexture2D* texture;
	static ConstructorHelpers::FObjectFinder<UTexture2D> theTextureFinder(TEXT("/Game/FirstPerson/Character/Textures/UE4_Mannequin_MAT_MASKA.UE4_Mannequin_MAT_MASKA"));
	if (theTextureFinder.Succeeded())
	{
		texture = theTextureFinder.Object;
	}

	//// ��̬���أ�ʹ�� FObjectFinderOptional ȥѰ��object������
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture> TextureFinder;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> MaterialFinder;

		FConstructorStatics():
			TextureFinder(TEXT("Texture2D'/Game/FirstPerson/Textures/FirstPersonCrosshair.FirstPersonCrosshair'")),
			MaterialFinder(TEXT("Material'/Game/FirstPerson/Meshes/BaseMaterial.BaseMaterial'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	auto T = ConstructorStatics.TextureFinder.Get();
	auto M = ConstructorStatics.MaterialFinder.Get();


	// use our custom HUD class
	HUDClass = AFirst2HUD::StaticClass();
}

void AFirst2GameMode::Sample1() const
{
	if (SoftObjectPath1.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__,
		       *FString("1 valid"));
	}
	if (SoftObjectPath2.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__,
		       *FString("2 null"));
	}

	if (SoftObjectPath3.IsAsset())
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__,
		       *FString("3 Asset"));
	}

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n%s\n    __________________"), __LINE__, *SoftObjectPath4.GetAssetName(), *SoftObjectPath4.GetAssetPathString());

	/* ��ӡ����ʾ����
	* LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 73
	1 valid
		__________________
	LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 84
	3 Asset
		__________________
	LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 87
	T_Brick_Clay_Beveled_N
	/Game/StarterContent/Textures/T_Brick_Clay_Beveled_N.T_Brick_Clay_Beveled_N
	 */
}

void AFirst2GameMode::REquestAsyncLoad_Example()
{
	// ������GetStreamableManager LoadSynchronous ��ʽͬ������ softobjectpath
	auto ob1 = UAssetManager::Get().GetStreamableManager().LoadSynchronous(SoftObjectPath3);

	// ������GetStreamableManager RequestSyncLoad ͬ������ softobjectpath
	UAssetManager::Get().GetStreamableManager().RequestSyncLoad(SoftObjectPath3);


	const auto handle = FStreamableDelegate::CreateUObject(this, &ThisClass::OnLoadFinish);
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(ArrayOfSoftObjects, handle);
}


void AFirst2GameMode::OnLoadFinish()
{
	for (auto asset : ArrayOfSoftObjects)
	{
		auto SoftObjPtr = FSoftObjectPtr(asset);
		auto Obj = SoftObjPtr.Get();
		if (Obj)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *Obj->GetName());
		}
	}
}

FString AFirst2GameMode::FuncWithParamAndRtv(int32 Number)
{
	return FString::FromInt(Number);
}

FString AFirst2GameMode::FuncWithParamAndRtv_Pure(int32 Number)
{
	return this->FuncWithParamAndRtv(Number);
}

/*����ԭ��CPP����*/
void AFirst2GameMode::CallCppInterface(AActor* SomeActor)
{
	UClass* ActorClass = SomeActor->GetClass();

	//���ActorClassʵ����StaticClass
	if (ActorClass->ImplementsInterface(USimpleIntf::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("implementation!"));

		ISimpleIntf* InterfaceIns = CastChecked<ISimpleIntf>(SomeActor); //�����ܲ���cast��ISimpleIntf

		// InterfaceIns->Inte
		if (InterfaceIns != NULL)
		{
			InterfaceIns->Interface_cpp(TEXT("wenjiezou"));
		}
	}
}

void AFirst2GameMode::CallNativeEventIntf(AActor* SomeActor)
{
	UClass* ActorClass = SomeActor->GetClass();

	//���ActorClassʵ����StaticClass
	if (ActorClass->ImplementsInterface(USimpleIntf::StaticClass()))
	{
		ISimpleIntf::Execute_NativeInterfa(SomeActor, TEXT("С�����"));
	}
}


void AFirst2GameMode::CallImplementationIntf(AActor* SomeActor)
{
	UClass* ActorClass = SomeActor->GetClass();

	//���ActorClassʵ����StaticClass
	if (ActorClass->ImplementsInterface(USimpleIntf::StaticClass()))
	{
		ISimpleIntf::Execute_ImplementableInterf(SomeActor, TEXT("С�����"));
	}
}

void AFirst2GameMode::TimerWorkExample()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFirst2GameMode::TimerFunction, 2.f, true);
	GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
	GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle);
	/*
	    ��ӡ���ʾ�⣺
		LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 145
		tick per 1 s
			__________________
		LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 145
		tick per 1 s
			__________________
		LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 145
		tick per 1 s
			__________________
	*/
}

void AFirst2GameMode::HowToNewObject()
{
	UTheCppObject* CppOb = NewObject<UTheCppObject>(); // Newһ��Object�ķ���
}

void AFirst2GameMode::HowToUseCustomLog()
{
	// �ṹ�������д���ϲ�Ҫдnew
	auto TR = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
	GetWorld()->SpawnActor<AActor>(TargetClas, TR);

	// ����Ĵ�����ʾ���������log����
	// �Զ���TheGMLog����ΪCategory
	UE_LOG(TheGMLog, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("gmlogexample"));
}

void AFirst2GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	HowToUseUniquePtr();

	
	
}


TSharedPtr<RawClas> AFirst2GameMode::HowToUseSharedPtr()
{
	// ���ٴ�������ָ��
	TSharedPtr<RawClas> simObjectPtr(new RawClas());

	TSharedPtr<RawClas> simObjectPtr2 = MakeShareable(new RawClas());

	// �����̰߳�ȫ��
	TSharedPtr<RawClas, ESPMode::ThreadSafe> simObjectPtr3 = MakeShareable(new RawClas());

	auto print = [simObjectPtr,simObjectPtr2, simObjectPtr3]()-> void
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\nsimObjectPtr[%d] simObjectPtr2[%d] simObjectPtr3[%d]\n    __________________"), __LINE__, simObjectPtr.GetSharedReferenceCount(), simObjectPtr2.GetSharedReferenceCount(), simObjectPtr3.GetSharedReferenceCount());
	};

	print();

	// ����ָ��
	simObjectPtr.Reset();
	simObjectPtr = nullptr;

	// ���ƹ���ָ��
	TSharedPtr<RawClas> copy = simObjectPtr;
	print();

	//ת�ƹ���ָ��
	TSharedPtr<RawClas> move = MoveTemp(simObjectPtr);
	print();

	//����
	if (copy) // �����ж�
	{
		copy->ExeFun();
	}
	if (copy.Get() != NULL)
	{
		copy.Get()->ExeFun(); // �����ã��������� ָ����õ���˼��
	}
	if (copy.IsValid())
	{
		(*copy).ExeFun(); // ͬ��Ҳ�ǽ�����
	}

	//�ȶ�ָ����ʲô���壿��
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString(copy==move? "==":"!="));
	return copy;
}


void AFirst2GameMode::HowToUseSharedRef()
{
	TSharedRef<RawClas> objRef(new RawClas());
	TSharedRef<RawClas> objRef2 = MakeShareable(new RawClas());
	TSharedRef<RawClas> objRef3 = MakeShared<RawClas>(); // ֱ�Ӱ������½��Ĺ���

	auto objPtr = HowToUseSharedPtr();
	TSharedPtr<RawClas> asP = objRef; // ��ʽת��

	// ����ת����
	TSharedRef<RawClas> asR = asP.ToSharedRef();

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString( objRef2 == objRef3 ? "==" : "!="));

	//�������ò������ǿ�
}

void AFirst2GameMode::HowToUseTWeakPtr()
{
	TSharedPtr<RawClas> p = MakeShared<RawClas>(); //��ӡ ����
	TWeakPtr<RawClas> w(p);

	/*lambda��ץ���ã������Ȳ�Ҫʹ��
	 * auto print = [p]()
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %dPtrCount: %d\n    __________________"), __LINE__, p.GetSharedReferenceCount());
	};*/

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %dPtrCount: %d\n    __________________"), __LINE__, p.GetSharedReferenceCount()); //��ӡ1

	TSharedRef<RawClas> r = MakeShareable(new RawClas()); //��ӡ ����
	TWeakPtr<RawClas> w2(r);

	//��������һ����ָ��
	TWeakPtr<RawClas> wcopy = w;
	//print();
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %dPtrCount: %d\n    __________________"), __LINE__, p.GetSharedReferenceCount()); //��ӡ1��Ҳ����p����


	if (w.IsValid())
	{
		//���p2�Ǿֲ���p2
		TSharedPtr<RawClas> p2 = w.Pin(); //��ʾȡ��ʵ��
		p2->ExeFun(); // ��ӡ��ExeFun
	}

	//���ǿָ��
	p.Reset();
	TSharedPtr<RawClas> p2 = w.Pin(); //��ʱw�Ѿ���destroy�ˣ�pin��������null
	if (!p2)
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("the weak pointer is empty ")); //�д�ӡ
	} //w�Ѿ���destory�ˣ�w���ʵ�屾��ǿգ���w.Pin�Ѿ��ǿյ��ˣ�

	//print();
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %dPtrCount: %d\n    __________________"), __LINE__, p.GetSharedReferenceCount()); //��ӡ0

	//��ָ�����õ����ַ�ʽ //��������ٿ���w2��ɶ
	w.Reset();
	w2 = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("the end "));
}

void AFirst2GameMode::HowToUseUniquePtr()
{
	TUniquePtr<RawClas> p = MakeUnique<RawClas>();
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\nvalidatity: %d\n    __________________"), __LINE__, p.IsValid());

	if (p.IsValid())
	{
		p->ExeFun();
	}
	TUniquePtr<RawClas> p2(p.Release());

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\np:%d, p2:%d\n    __________________"), __LINE__,  p.IsValid(), p2.IsValid() );

	p.Reset();
	p2=nullptr;

	
}

/*ָ��ת����ʾ��*/
void AFirst2GameMode::CastExample()
{
	TSharedPtr<RawClas> simpleObj;
	TSharedPtr<ComplexClas> complexObj=MakeShared<ComplexClas>(); //�½��ķ�ʽ
	simpleObj=complexObj; //������ת��Ϊ���ࡣֱ�Ӹ�ֵ��
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\nsimple obje is valid: %s\n    __________________"), __LINE__, simpleObj.IsValid()?TEXT("VALID"):TEXT("INVALID"));
	
	TSharedPtr<ComplexClas> complexObj2=StaticCastSharedPtr<ComplexClas>(simpleObj); //����ת��Ϊ������
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\ncomplext obj2 is valid: %s\n    __________________"), __LINE__, complexObj2.IsValid()?TEXT("VALID"):TEXT("INVALID"));

	const TSharedPtr<RawClas>constSimpleO(new RawClas());
	TSharedPtr<RawClas>mutableSimpleO=ConstCastSharedPtr<RawClas>(constSimpleO); //����ָ��ת��Ϊ�ǳ�����ָ��

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\nmutable obj is valid: %s\n    __________________"), __LINE__, mutableSimpleO.IsValid()?TEXT("VALID"):TEXT("INVALID"));

}

void AFirst2GameMode::HowToCastRef()
{
	TSharedRef<ComplexClas> C(new ComplexClas());

	TSharedRef<RawClas> R ;
	R = C; //��ʽת��������ת��Ϊ����
	
	TSharedRef<ComplexClas> C2;
	C2 = StaticCastSharedRef<ComplexClas>(R); //����ת��Ϊ����
	
	const TSharedRef<ComplexClas> constC = MakeShareable(new ComplexClas());
	C2=ConstCastSharedRef<ComplexClas>(constC); //����ָ�� �� ��ָͨ��
	
	
}

/*
 * ���ʹ��TMap��TSet
 */
void AFirst2GameMode::HowToUseTMapAndTSet()
{
	TMap<int32, FString> IntStrMap{};
	IntStrMap.Add(1, TEXT("UltraMan"));
	IntStrMap.Emplace(2, TEXT("PunchMan"));
	FString Value = IntStrMap.FindOrAdd(1); //û�д�Ĭ����ʲôֵ����ô����FString��Ĭ��ֵ
	IntStrMap.Remove(1);
	auto num = IntStrMap.Num();
	if (IntStrMap.Contains(1))
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *Value);
	}
	TArray<int32> Keys;
	TArray<FString> Values;
	IntStrMap.GenerateKeyArray(Keys);
	IntStrMap.GenerateValueArray(Values);

	IntStrMap.Empty();
	IntStrMap.Reset();

	for (auto elem : IntStrMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%d:%s\n    __________________"), __LINE__, elem.Key, * elem.Value);
	}

	for (auto it = IntStrMap.CreateIterator(); it; ++it)
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%d:%s | %d:%s\n    __________________"), __LINE__, it.Key(), * it.Value(), it->Key, *it->Value); // ���ַ��ʷ�ʽ
	}

	TMultiMap<int32, FString> IntStrMultiMap{};

	IntStrMultiMap.Add(1, "2");
	IntStrMultiMap.Add(1, "23");

	for (auto it = IntStrMap.CreateIterator(); it; ++it)
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%d:%s\n    __________________"), __LINE__, it.Key(), *it.Value());
	}

	TSet<FString> StrSet;
	StrSet.Add(TEXT("Banana"));
	StrSet.Add(TEXT("Banana"));
	StrSet.Add(TEXT("Pineaple"));
	StrSet.Remove(TEXT("Banana"));
	int32 len = StrSet.Num();
	bool Contains = StrSet.Contains(TEXT("Banana"));
	FString* PtrBanana = StrSet.Find(TEXT("Pineaple"));

	*PtrBanana = TEXT("Pine2"); // ʹ��Find������ָ�룬��������

	for (auto elem : StrSet)
	{
		// elem��FString����
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *elem);
	}
	for (auto It = StrSet.CreateConstIterator(); It; ++It) // ���������ܺ�Ӽӣ���ֻ����ǰ�Ӽ�
	{
		//*It����FString
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, * *It);
	}
}


void AFirst2GameMode::TimerFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("tick per 1 s"));
}

/*
 * ǰ̨���� + �Զ�����
 */
void AFirst2GameMode::Excample_FNonAban1()
{
	auto AutoDeleteIns = new FAutoDeleteAsyncTask<FADTask>(3);
	AutoDeleteIns->StartSynchronousTask();
}

/*
 * ǰ̨���� + �ֶ����գ����Զ�����
 */
void AFirst2GameMode::Excample_FNonAban_Manu()
{
	ManTask = new FAsyncTask<FMDTask>(3); // new���ص���ָ��
	ManTask->StartSynchronousTask();

	// ÿ2s����һ���Ƿ�Ҫ���գ�Ҳ�����ֶ����յ��Զ�����
	if (!GetWorld()->GetTimerManager().IsTimerActive(TH))
	{
		GetWorld()->GetTimerManager().SetTimer(TH, this, &ThisClass::TryToRecollectTask, 2.f, true);
	}
}

void AFirst2GameMode::TryToRecollectTask()
{
	if (ManTask->IsDone())
	{
		delete ManTask;
		ManTask = nullptr;

		GetWorld()->GetTimerManager().ClearTimer(TH);
	}
}

/*
 * ��̨���� + �Զ�����
 */
void AFirst2GameMode::Excample_FNonAban1_BG()
{
	auto AutoDeleteIns = new FAutoDeleteAsyncTask<FADTask>(3);
	AutoDeleteIns->StartBackgroundTask();
}

/*
 * ��̨���� + �ֶ����գ����Զ�����
 */
void AFirst2GameMode::Excample_FNonAban_Manu_BG()
{
	ManTask = new FAsyncTask<FMDTask>(3); // new���ص���ָ��
	ManTask->StartBackgroundTask();

	if (!GetWorld()->GetTimerManager().IsTimerActive(TH))
	{
		// ���շ�ʽ��һ��������ʹ��IsDone���ж�
		GetWorld()->GetTimerManager().SetTimer(TH, this, &ThisClass::TryToRecollectTask, 2.f, true);
	}
}

/*
	�� E:\pro\First2\First2\Source\First2\First2.Build.cs ����Ҫ���ģ��
	PrivateDependencyModuleNames.AddRange(new string[] { "XmlParser" });
 */
void AFirst2GameMode::XMLReadExampl()
{
	// �漰·����Ҫע��
	FXmlFile* x = new FXmlFile("E:/pro/First2/First2/Saved/BookLibr.xml");
	FXmlNode* rootnode = x->GetRootNode();
	FXmlNode* bookname = rootnode->FindChildNode("bookname");
	FString bookcategory = bookname->GetAttribute("category");
	FXmlNode* author = rootnode->FindChildNode("author");
	FString authorsex = author->GetAttribute("sex");
	FXmlNode* bookprice = rootnode->FindChildNode("price");
	FString pricecurrency = bookprice->GetAttribute("currency");

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s %s %s \n    __________________"), __LINE__, *bookcategory, *authorsex, *pricecurrency);
}

void AFirst2GameMode::JsonReadExample()
{
	/* ��Ӧ���½ṹ 
	{
		"weight": 130,
		"hobby": "GAME",
		"BasicInfo": {
			"name": "tom",
			"age": 18,
			"IsMan": true
		},
		"children": [
			{
				"name": "tom",
				"age": 18,
				"IsMan": true
			},
			{
				"name": "elsa",
				"age": 25,
				"IsMan": false
			}
		]
	}*/
	JsonOb = MakeShareable(new FJsonObject);
	JsonOb->SetStringField(TEXT("name"),TEXT("tom"));
	JsonOb->SetNumberField(TEXT("age"), 18);
	JsonOb->SetBoolField(TEXT("IsMan"), true);

	JsonObBig = MakeShareable(new FJsonObject);
	JsonObBig->SetNumberField(TEXT("weight"), 130);
	JsonObBig->SetStringField(TEXT("hobby"),TEXT("GAME"));
	JsonObBig->SetObjectField(TEXT("BasicInfo"), JsonOb); // Ƕ������

	TSharedPtr<FJsonObject> j1 = MakeShareable(new FJsonObject);
	j1->SetStringField(TEXT("name"),TEXT("tom"));
	j1->SetNumberField(TEXT("age"), 18);
	j1->SetBoolField(TEXT("IsMan"), true);

	TSharedPtr<FJsonObject> j2 = MakeShareable(new FJsonObject);
	j2->SetStringField(TEXT("name"),TEXT("elsa"));
	j2->SetNumberField(TEXT("age"), 25);
	j2->SetBoolField(TEXT("IsMan"), false);

	JsonValueArray.Add(MakeShareable(new FJsonValueObject(j1)));
	JsonValueArray.Add(MakeShareable(new FJsonValueObject(j2)));
	// UE4_C++ ƴ��JsonǶ�׽ṹ_SimpleCloud94�Ĳ���-CSDN����_c++ json Ƕ�� https://blog.csdn.net/longyanbuhui/article/details/122070314


	JsonObBig->SetArrayField(TEXT("children"), JsonValueArray);
	FString outString;
	auto writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&outString);
	FJsonSerializer::Serialize(JsonObBig.ToSharedRef(), writer);

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *outString);

	/* ��ӡ����� LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 349
{"weight":130,"hobby":"GAME","BasicInfo":{"name":"tom","age":18,"IsMan":true},"children":[{"name":"tom","age":18,"IsMan":true},{"name":"elsa","age":25,"IsMan":false}]}
	__________________*/
}
