// Fill out your copyright notice in the Description page of Project Settings.


#include "Imperium_Functions.h"
#include "JsonBlueprintFunctionLibrary.h"
#include "Imperium_Assets.h"
#include "Imperium_Settings.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IPluginManager.h"

UImperium_Project* UImperium_Functions::GetImperiumProjectAsset()
{
	if(UObject* _out = GetMutableDefault<UImperium_DevSettings>()->DefaultSettings_Project.TryLoad())
	{
		return Cast<UImperium_Project>(_out);
	}
	return nullptr;
}

FString UImperium_Functions::GetImperiumPluginContentPath()
{
	
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("ImperiumDB");
    
	if (Plugin.IsValid())
	{
		// Get the base directory of the plugin
		FString PluginBaseDir = Plugin->GetBaseDir();
        
		// Append "Content" to get the content folder path
		FString PluginContentDir = FPaths::Combine(PluginBaseDir, TEXT("Content"));
        
		return PluginContentDir;
	}
    
	return FString();
}

UImperium_NodeSettings* UImperium_Functions::GetImperiumNodeSettings()
{
	if(UImperium_Project* _set= GetImperiumProjectAsset())
	{
		return _set->NodeSettings;
	}
	return nullptr;
}

FImperium_AssetData UImperium_Functions::GotNodeData_FromLabel(FImperium_Flow flowData, const FString& label)
{
	for(auto f : flowData.Nodes)
	{
		if(f.Label==label)
		{
		return f;
		}
	}
	return FImperium_AssetData();
}

FImperium_Flow UImperium_Functions::LoadImpFlow(const FString& flowName)
{
	FFilePath _path;
	_path.FilePath=GetPathToFlow(flowName);
	FJsonObjectWrapper _json;
	UJsonBlueprintFunctionLibrary::FromFile(_path,_json);
	return Conv_JsonToFlow(_json,flowName);
}

FImperium_Flow UImperium_Functions::Conv_JsonToFlow(const FJsonObjectWrapper Json, const FString& Label)
{
	FImperium_Flow _newDat;
	_newDat.Label=Label;

	//set meta or data from flow asset
	TSharedPtr<FJsonObject> _meta=Json.JsonObject;
	if(Json.JsonObject->HasField("meta"))
	{
		_meta=Json.JsonObject->GetObjectField("meta");
	}
	
	TArray<TSharedPtr<FJsonValue>> j_con=_meta->GetArrayField("connections");
	for(TSharedPtr<FJsonValue> j : j_con)
	{
		FImperium_Connection _newCon;
		FJsonObject* _tbl=j.Get()->AsObject().Get();
		_newCon.from_node=_tbl->GetStringField("from_node");
		_newCon.to_node=_tbl->GetStringField("to_node");
		_newCon.from_port=_tbl->GetIntegerField("from_port");
		_newCon.to_port=_tbl->GetIntegerField("to_port");
		_newDat.Connections.Add(_newCon);
	}
	TArray<TSharedPtr<FJsonValue>> j_nod=_meta->GetArrayField("nodes");
	for(TSharedPtr<FJsonValue> j : j_nod)
	{
		FImperium_AssetData _newNod;
		_newNod.Template=j.Get()->AsObject().Get()->GetStringField("_template");
		_newNod.Label=j.Get()->AsObject().Get()->GetStringField("label");
		_newNod.key=FName(*j.Get()->AsObject().Get()->GetStringField("key"));
		//_newNod.Label=j.Get()->AsObject().Get()->GetStringField("label");
		_newNod.meta.JsonObject=j.Get()->AsObject().Get()->GetObjectField("meta");
		_newNod.params.JsonObject=j.Get()->AsObject().Get()->GetObjectField("params");
		TArray<FString> _tagsTemp;
		j.Get()->AsObject()->TryGetStringArrayField("tags",_tagsTemp);
		for(FString t : _tagsTemp)
		{
			_newNod.tags.Add(*t);
		}

		_newDat.Nodes.Add(_newNod);
	}
	return _newDat;
}




TMap<FString, FString> UImperium_Functions::GetFilesInPath(const FString& Path, const FString& Extension,
	bool bRecursive)
{
	TMap<FString, FString> FileMap;
    
	// Prepare the search pattern
	FString SearchPattern = Path / TEXT("*") + Extension;
    
	// Array to hold found files
	TArray<FString> FoundFiles;
    
	// Use the file manager to find files
	IFileManager& FileManager = IFileManager::Get();
    
	if (bRecursive)
	{
		FileManager.FindFilesRecursive(FoundFiles, *Path, *FString(TEXT("*") + Extension), true, false);
	}
	else
	{
		FileManager.FindFiles(FoundFiles, *SearchPattern, true, false);
	}
    
	// Process each found file
	for (const FString& FilePath : FoundFiles)
	{
		// Get the full path
		FString FullPath;
		if (bRecursive)
		{
			// FindFilesRecursive returns full paths
			FullPath = FilePath;
		}
		else
		{
			// FindFiles returns relative names, need to construct full path
			FullPath = Path / FilePath;
		}
        
		// Extract filename without extension
		FString FileName = FPaths::GetBaseFilename(FullPath);
        
		// Add to map
		FileMap.Add(FileName, FullPath);
	}
    
	return FileMap;
}

FString UImperium_Functions::GetPathToFlow(const FString& FlowName)
{
	if(UImperium_Project* _proj=GetImperiumProjectAsset())
	{
		TMap<FString,FString> _fils=GetFilesInPath(_proj->GetDirectory_Flow(),TEXT(".ImpFlow"),true);
		TArray<FString> _nams;
		_fils.GetKeys(_nams);
		for (FString s : _nams)
		{
			if(s==FlowName)
			{
				return _fils.FindOrAdd(s);
			}
		}
	}
	return "";
}
