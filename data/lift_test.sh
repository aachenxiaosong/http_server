#call lift
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/callLift --data '{"homeId":"2094475", "unlockTime":300}'
#book lift
curl -H "Content-Type:application/json" -X POST http://127.0.0.1:8080/liftCtrl/v3/bookLift --data '{"defaultHomeId":"2094475", "authorizedHomeIds":["2094474"],"mode":"unlock","deviceCode":"hf0000124","unlockTime":300}'
curl -H "Content-Type:application/json" -X POST http://127.0.0.1:8080/liftCtrl/v3/bookLift --data '{"defaultHomeId":"2094475", "authorizedHomeIds":["2094474"],"mode":"open","deviceCode":"hf0000124","unlockTime":300}'
curl -H "Content-Type:application/json" -X POST http://127.0.0.1:8080/liftCtrl/v3/bookLift --data '{"defaultHomeId":"2094475", "authorizedHomeIds":["2094474"],"mode":"none","deviceCode":"hf0000124","unlockTime":300}'
curl -H "Content-Type:application/json" -X POST http://127.0.0.1:8080/liftCtrl/v3/bookLift --data '{"defaultHomeId":"2094475", "authorizedHomeIds":["2094474"],"mode":"all","deviceCode":"hf0000124","unlockTime":300}'
curl -H "Content-Type:application/json" -X POST http://127.0.0.1:8080/liftCtrl/v3/bookLift --data '{"defaultHomeId":"2094475", "authorizedHomeIds":["2094474"],"mode":"none","deviceCode":"hf0000124"}'
curl -H "Content-Type:application/json" -X POST http://127.0.0.1:8080/liftCtrl/v3/bookLift --data '{"defaultHomeId":"2094475", "authorizedHomeIds":["2094474"],"mode":"none"}'
#book lift inter
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/bookLiftInter --data '{"sHomeId":"2094475","dHomeId":"2094474"}'
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/bookLiftInter --data '{"sHomeId":"2094475","dHomeId":"2094474"}'
#take lift
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/bookLiftInter --data '{"sHomeId":"2094475","dHomeId":"2094474"}'
#lift status
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/liftStatus --data '{"homeId":"2094475", "elevatorId":1}'

#wechat
#ctrl
{
	"header": {
		"brand": "yzs",
		"method": "device-ubox-lift-book",
		"payloadVersion": 1,
		"timestamp": 1606902478251,
		"traceId": "62af0b91-7966-49a1-9a03-17418a119f92"
	},
	"payload":	{
		"deviceCode": "7ccbe2e29772",
		"reqId": "6e976f71b396417c8edcdd51b77a4741",
		"timestamp": 1606902190000,
		"bookType": "wechat",
		"bookData": {
			"homeId": "2094475",
			"fromFloor": "1",
			"toFloor": "5"
		}
	}
}
