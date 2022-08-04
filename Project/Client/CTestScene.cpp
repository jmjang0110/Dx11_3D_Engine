#include "pch.h"
#include "CTestScene.h"

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider2D.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCamera.h>
#include <Engine/CLight2D.h>

#include <Engine/CSceneFile.h>
#include <Engine/CSound.h>

#include <Script/CPlayerScript.h>
#include <Script/CCameraMoveScript.h>
#include <Script/CMissileScript.h>

#include <Script\CSceneSaveLoad.h>



void CTestScene::CreateTestScene()
{
	//CResMgr::GetInst()->Load<CSceneFile>(L"scene\\Test.scene", L"scene\\Test.scene");
	//CResMgr::GetInst()->Load<CSceneFile>(L"scene\\Test_01.scene", L"scene\\Test_01.scene");

	//CResMgr::GetInst()->Load<CTexture>(L"texture\\deadcell.png", L"texture\\deadcell.png");
	//CResMgr::GetInst()->Load<CTexture>(L"texture\\deadcell_n.png", L"texture\\deadcell_n.png");

	Ptr<CSound> pSound = CResMgr::GetInst()->Load<CSound>(L"sound\\BGM_Stage1.wav", L"sound\\BGM_Stage1.wav");
	pSound->Play(1, 0.5f);

	CScene* pCurScene = new CScene;
	CSceneMgr::GetInst()->ChangeScene(pCurScene);		

	//return;

	pCurScene->SetLayerName(0, L"Tile");
	pCurScene->SetLayerName(1, L"Default");
	pCurScene->SetLayerName(2, L"Player");
	pCurScene->SetLayerName(3, L"Monster");

	// Texture 한장 로딩해보기
	CResMgr::GetInst()->Load<CTexture>(L"texture\\Player.bmp", L"texture\\Player.bmp");
	CResMgr::GetInst()->Load<CTexture>(L"texture\\MagicCircle.png", L"texture\\MagicCircle.png");


	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();

	pCurScene->AddObject(pCamObj, L"Default");


	// 광원 오브젝트 추가
	CGameObject* pLight2D = new CGameObject;
	pLight2D->SetName(L"DirLight");

	pLight2D->AddComponent(new CTransform);
	pLight2D->AddComponent(new CMeshRender);
	pLight2D->AddComponent(new CLight2D);

	pLight2D->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pLight2D->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, -XM_PI / 4.f));

	pLight2D->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);	
	pLight2D->Light2D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));

	pCurScene->AddObject(pLight2D, L"Default");


	//pLight2D = new CGameObject;
	//pLight2D->SetName(L"PointLight");

	//pLight2D->AddComponent(new CTransform);
	//pLight2D->AddComponent(new CMeshRender);
	//pLight2D->AddComponent(new CLight2D);

	//pLight2D->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	//pLight2D->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pLight2D->Light2D()->SetRange(500.f);
	//pLight2D->Light2D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));	

	//pCurScene->AddObject(pLight2D, L"Default");


	// Background Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Background");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 500.f);
	pObject->Transform()->SetRelativeScale(4096.f, 8192.f, 1.f);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\deadcell.png"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\deadcell_n.png"));

	pCurScene->AddObject(pObject, L"Default");

	// Prefab 만들기
	/*if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Background.pref"))
	{
		CPrefab* pPrefab = new CPrefab;
		pPrefab->SetProto(pObject->Clone());
		CResMgr::GetInst()->AddRes<CPrefab>(L"prefab\\Background.pref", pPrefab);
	}*/

	// Particle Object 
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"ParticleObject_01");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->Transform()->SetRelativePos(0.f, 0.f, 500.f);

	Ptr<CTexture> pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\particle\\AlphaCircle.png", L"texture\\particle\\AlphaCircle.png");
	pParticleObj->ParticleSystem()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pParticleTex);

	pCurScene->AddObject(pParticleObj, L"Default");


	pParticleObj = pParticleObj->Clone();
	pParticleObj->SetName(L"ParticleObject_02");
	pParticleObj->Transform()->SetRelativePos(-500.f, 0.f, 500.f);
	pCurScene->AddObject(pParticleObj, L"Default");




	// PostProcess Object
	CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcessObject");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pPostProcess->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	pCurScene->AddObject(pPostProcess, L"Default");
	pPostProcess->Deactivate();


	((CCameraMoveScript*)pCamObj->GetScript(0))->SetFilter(pPostProcess);
	CCameraMoveScript* pCamMoveScript = pCamObj->GetScript<CCameraMoveScript>();





	// 충돌 레이어 설정
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");	
	
	pCurScene->SetResKey(L"scene\\TestScene.scene");
	wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();	
	CSceneSaveLoad::SaveScene(pCurScene, strSceneFilePath + L"scene\\TestScene.scene");
	
	pCurScene->start();	
	pCurScene->SetSceneState(SCENE_STATE::PLAY);
}
