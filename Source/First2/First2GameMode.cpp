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


// DEFINE_LOG_CATEGORY(TheGMLog) 和 DECLARE_LOG_CATEGORY_EXTERN(TheGMLog, Log, All) 是配套使用的
DEFINE_LOG_CATEGORY(TheGMLog);

AFirst2GameMode::AFirst2GameMode()
	: Super()
{
	////////////////////////////////////////////////////////////////////////// 展示了静态加载的代码

	// ClassFinder的用法1：没有blueprint前缀与.xx
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// ClassFinder的用法2：具有blueprint前缀，并且使用 X.X_C 的格式去加载
	static ConstructorHelpers::FClassFinder<APawn> C(TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter_C'"));
	DefaultPawnClass = C.Class;


	// StaticLoadObject 动态加载 会崩溃：暂时没有理解为什么崩溃，网上说是OK的，可能是在构造函数里才会崩溃吧。
	// [UE4]C++实现动态加载UObject：StaticLoadObject()；以Texture和Material为例-pudn.com https://www.pudn.com/news/628f8299bf399b7f351e34fc.html

	// 找到Class的第三个方法 
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

	//// 动态加载，使用 FObjectFinderOptional 去寻找object并加载
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

	/* 打印内容示例：
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
	// 案例：GetStreamableManager LoadSynchronous 方式同步加载 softobjectpath
	auto ob1 = UAssetManager::Get().GetStreamableManager().LoadSynchronous(SoftObjectPath3);

	// 案例：GetStreamableManager RequestSyncLoad 同步加载 softobjectpath
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

/*调用原生CPP代码*/
void AFirst2GameMode::CallCppInterface(AActor* SomeActor)
{
	UClass* ActorClass = SomeActor->GetClass();

	//如果ActorClass实现了StaticClass
	if (ActorClass->ImplementsInterface(USimpleIntf::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("implementation!"));

		ISimpleIntf* InterfaceIns = CastChecked<ISimpleIntf>(SomeActor); //看看能不能cast到ISimpleIntf

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

	//如果ActorClass实现了StaticClass
	if (ActorClass->ImplementsInterface(USimpleIntf::StaticClass()))
	{
		ISimpleIntf::Execute_NativeInterfa(SomeActor, TEXT("小江村儿"));
	}
}


void AFirst2GameMode::CallImplementationIntf(AActor* SomeActor)
{
	UClass* ActorClass = SomeActor->GetClass();

	//如果ActorClass实现了StaticClass
	if (ActorClass->ImplementsInterface(USimpleIntf::StaticClass()))
	{
		ISimpleIntf::Execute_ImplementableInterf(SomeActor, TEXT("小江村儿"));
	}
}

void AFirst2GameMode::TimerWorkExample()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFirst2GameMode::TimerFunction, 2.f, true);
	GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
	GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle);
	/*
	    打印结果示意：
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
	UTheCppObject* CppOb = NewObject<UTheCppObject>(); // New一个Object的方法
}

void AFirst2GameMode::HowToUseCustomLog()
{
	// 结构体的声明写法上不要写new
	auto TR = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
	GetWorld()->SpawnActor<AActor>(TargetClas, TR);

	// 下面的代码演示了如何新增log类型
	// 自定义TheGMLog来作为Category
	UE_LOG(TheGMLog, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("gmlogexample"));
}

void AFirst2GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	HowToUseUniquePtr();

	
	
}


TSharedPtr<RawClas> AFirst2GameMode::HowToUseSharedPtr()
{
	// 快速创建共享指针
	TSharedPtr<RawClas> simObjectPtr(new RawClas());

	TSharedPtr<RawClas> simObjectPtr2 = MakeShareable(new RawClas());

	// 这是线程安全的
	TSharedPtr<RawClas, ESPMode::ThreadSafe> simObjectPtr3 = MakeShareable(new RawClas());

	auto print = [simObjectPtr,simObjectPtr2, simObjectPtr3]()-> void
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\nsimObjectPtr[%d] simObjectPtr2[%d] simObjectPtr3[%d]\n    __________________"), __LINE__, simObjectPtr.GetSharedReferenceCount(), simObjectPtr2.GetSharedReferenceCount(), simObjectPtr3.GetSharedReferenceCount());
	};

	print();

	// 重置指针
	simObjectPtr.Reset();
	simObjectPtr = nullptr;

	// 复制共享指针
	TSharedPtr<RawClas> copy = simObjectPtr;
	print();

	//转移共享指针
	TSharedPtr<RawClas> move = MoveTemp(simObjectPtr);
	print();

	//调用
	if (copy) // 条件判断
	{
		copy->ExeFun();
	}
	if (copy.Get() != NULL)
	{
		copy.Get()->ExeFun(); // 解引用（解引用有 指针调用的意思）
	}
	if (copy.IsValid())
	{
		(*copy).ExeFun(); // 同样也是解引用
	}

	//比对指针是什么含义？？
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString(copy==move? "==":"!="));
	return copy;
}


void AFirst2GameMode::HowToUseSharedRef()
{
	TSharedRef<RawClas> objRef(new RawClas());
	TSharedRef<RawClas> objRef2 = MakeShareable(new RawClas());
	TSharedRef<RawClas> objRef3 = MakeShared<RawClas>(); // 直接包含了新建的功能

	auto objPtr = HowToUseSharedPtr();
	TSharedPtr<RawClas> asP = objRef; // 隐式转化

	// 反向转换：
	TSharedRef<RawClas> asR = asP.ToSharedRef();

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString( objRef2 == objRef3 ? "==" : "!="));

	//共享引用不可以是空
}

void AFirst2GameMode::HowToUseTWeakPtr()
{
	TSharedPtr<RawClas> p = MakeShared<RawClas>(); //打印 构造
	TWeakPtr<RawClas> w(p);

	/*lambda会抓引用，所以先不要使用
	 * auto print = [p]()
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %dPtrCount: %d\n    __________________"), __LINE__, p.GetSharedReferenceCount());
	};*/

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %dPtrCount: %d\n    __________________"), __LINE__, p.GetSharedReferenceCount()); //打印1

	TSharedRef<RawClas> r = MakeShareable(new RawClas()); //打印 构造
	TWeakPtr<RawClas> w2(r);

	//单独拷贝一份弱指针
	TWeakPtr<RawClas> wcopy = w;
	//print();
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %dPtrCount: %d\n    __________________"), __LINE__, p.GetSharedReferenceCount()); //打印1，也就是p本身


	if (w.IsValid())
	{
		//这个p2是局部的p2
		TSharedPtr<RawClas> p2 = w.Pin(); //表示取到实体
		p2->ExeFun(); // 打印了ExeFun
	}

	//清空强指针
	p.Reset();
	TSharedPtr<RawClas> p2 = w.Pin(); //此时w已经被destroy了，pin出来的是null
	if (!p2)
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("the weak pointer is empty ")); //有打印
	} //w已经被destory了（w这个实体本身非空，而w.Pin已经是空的了）

	//print();
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %dPtrCount: %d\n    __________________"), __LINE__, p.GetSharedReferenceCount()); //打印0

	//弱指针重置的两种方式 //这里可以再看看w2是啥
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

/*指针转换的示例*/
void AFirst2GameMode::CastExample()
{
	TSharedPtr<RawClas> simpleObj;
	TSharedPtr<ComplexClas> complexObj=MakeShared<ComplexClas>(); //新建的方式
	simpleObj=complexObj; //派生类转换为基类。直接赋值。
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\nsimple obje is valid: %s\n    __________________"), __LINE__, simpleObj.IsValid()?TEXT("VALID"):TEXT("INVALID"));
	
	TSharedPtr<ComplexClas> complexObj2=StaticCastSharedPtr<ComplexClas>(simpleObj); //基类转换为派生类
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\ncomplext obj2 is valid: %s\n    __________________"), __LINE__, complexObj2.IsValid()?TEXT("VALID"):TEXT("INVALID"));

	const TSharedPtr<RawClas>constSimpleO(new RawClas());
	TSharedPtr<RawClas>mutableSimpleO=ConstCastSharedPtr<RawClas>(constSimpleO); //常量指针转换为非常量的指针

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\nmutable obj is valid: %s\n    __________________"), __LINE__, mutableSimpleO.IsValid()?TEXT("VALID"):TEXT("INVALID"));

}

void AFirst2GameMode::HowToCastRef()
{
	TSharedRef<ComplexClas> C(new ComplexClas());

	TSharedRef<RawClas> R ;
	R = C; //隐式转换：子类转换为基类
	
	TSharedRef<ComplexClas> C2;
	C2 = StaticCastSharedRef<ComplexClas>(R); //基类转换为子类
	
	const TSharedRef<ComplexClas> constC = MakeShareable(new ComplexClas());
	C2=ConstCastSharedRef<ComplexClas>(constC); //常量指针 → 普通指针
	
	
}

/*
 * 如何使用TMap和TSet
 */
void AFirst2GameMode::HowToUseTMapAndTSet()
{
	TMap<int32, FString> IntStrMap{};
	IntStrMap.Add(1, TEXT("UltraMan"));
	IntStrMap.Emplace(2, TEXT("PunchMan"));
	FString Value = IntStrMap.FindOrAdd(1); //没有传默认设什么值，那么就是FString的默认值
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
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%d:%s | %d:%s\n    __________________"), __LINE__, it.Key(), * it.Value(), it->Key, *it->Value); // 两种访问方式
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

	*PtrBanana = TEXT("Pine2"); // 使用Find出来的指针，调整内容

	for (auto elem : StrSet)
	{
		// elem是FString本体
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *elem);
	}
	for (auto It = StrSet.CreateConstIterator(); It; ++It) // 迭代器不能后加加，而只能是前加加
	{
		//*It才是FString
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, * *It);
	}
}


void AFirst2GameMode::TimerFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("tick per 1 s"));
}

/*
 * 前台启动 + 自动回收
 */
void AFirst2GameMode::Excample_FNonAban1()
{
	auto AutoDeleteIns = new FAutoDeleteAsyncTask<FADTask>(3);
	AutoDeleteIns->StartSynchronousTask();
}

/*
 * 前台启动 + 手动回收（的自动化）
 */
void AFirst2GameMode::Excample_FNonAban_Manu()
{
	ManTask = new FAsyncTask<FMDTask>(3); // new返回的是指针
	ManTask->StartSynchronousTask();

	// 每2s决定一下是否要回收，也算是手动回收的自动化。
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
 * 后台启动 + 自动回收
 */
void AFirst2GameMode::Excample_FNonAban1_BG()
{
	auto AutoDeleteIns = new FAutoDeleteAsyncTask<FADTask>(3);
	AutoDeleteIns->StartBackgroundTask();
}

/*
 * 后台启动 + 手动回收（的自动化）
 */
void AFirst2GameMode::Excample_FNonAban_Manu_BG()
{
	ManTask = new FAsyncTask<FMDTask>(3); // new返回的是指针
	ManTask->StartBackgroundTask();

	if (!GetWorld()->GetTimerManager().IsTimerActive(TH))
	{
		// 回收方式都一样，都是使用IsDone的判断
		GetWorld()->GetTimerManager().SetTimer(TH, this, &ThisClass::TryToRecollectTask, 2.f, true);
	}
}

/*
	在 E:\pro\First2\First2\Source\First2\First2.Build.cs 中需要添加模块
	PrivateDependencyModuleNames.AddRange(new string[] { "XmlParser" });
 */
void AFirst2GameMode::XMLReadExampl()
{
	// 涉及路径需要注意
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
	/* 对应如下结构 
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
	JsonObBig->SetObjectField(TEXT("BasicInfo"), JsonOb); // 嵌套类型

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
	// UE4_C++ 拼接Json嵌套结构_SimpleCloud94的博客-CSDN博客_c++ json 嵌套 https://blog.csdn.net/longyanbuhui/article/details/122070314


	JsonObBig->SetArrayField(TEXT("children"), JsonValueArray);
	FString outString;
	auto writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&outString);
	FJsonSerializer::Serialize(JsonObBig.ToSharedRef(), writer);

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *outString);

	/* 打印结果： LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 349
{"weight":130,"hobby":"GAME","BasicInfo":{"name":"tom","age":18,"IsMan":true},"children":[{"name":"tom","age":18,"IsMan":true},{"name":"elsa","age":25,"IsMan":false}]}
	__________________*/
}
