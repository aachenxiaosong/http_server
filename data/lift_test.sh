#call lift
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/callLift --data '{"homeId":"2094475", "unlockTime":300}'
#book lift
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/bookLift --data '{"homeId":"2094475", "deviceCode":"hf0000124","unlockTime":300}'
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/bookLift --data '{"homeId":"2094475", "deviceCode":"hf0000124"}'
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/bookLift --data '{"homeId":"2094475"}'
#book lift inter
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/bookLiftInter --data '{"sHomeId":"2094475","dHomeId":"2094474"}'
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/bookLiftInter --data '{"sHomeId":"2094475","dHomeId":"2094474"}'
#take lift
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/bookLiftInter --data '{"sHomeId":"2094475","dHomeId":"2094474"}'
#lift status
curl -H "Content-Type:application/json" -X POST http://192.168.6.10:8080/liftCtrl/v3/liftStatus --data '{"homeId":"2094475", "elevatorId":1}'
