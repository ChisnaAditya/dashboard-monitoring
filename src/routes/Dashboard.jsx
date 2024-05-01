import Card from "../components/Card";
import relay from "../assets/relay.svg";
import suhu from "../assets/suhu.svg";
import oksigen from "../assets/do.svg";
import ph from "../assets/ph.svg";
import user from "../assets/user.svg";

import { ref, onValue } from "firebase/database";
import { database } from "../firebase";
import { useEffect, useState } from "react";

export default function Dashboard() {
  const [reading, setReading] = useState([]);
  const [monitoring, setMonitoring] = useState([]);

  const db = database;
  const path = ref(db, "database");

  useEffect(() => {
    onValue(path, (snapshot) => {
      const data = snapshot.val();
      // console.log(data);
      setReading(data.reading);
      setMonitoring(data.monitoring);
    });
  }, []);

  return (
    <div>
      <div className="container flex items-center justify-evenly h-24 bg-pattern rounded-b-[40px] shadow-xl">
        <h1 className="text-4xl text-black font-bold">Dashboard</h1>
        <div className="user">
          <img
            alt="user"
            src={user}
            className="w-[40px] rounded-full bg-indigo-600 p-1"
          />
        </div>
      </div>
      <section className="container py-5">
        <div className="pb-2 font-bold px-2">Sensors</div>
        <div className="flex flex-col gap-4">
          <Card
            logo={suhu}
            title="Suhu Air"
            value={monitoring.SUHU.toFixed(1)}
          />
          <Card
            logo={oksigen}
            title="DO (Oksigen Terlarut)"
            value={monitoring.DO}
          />
          <Card logo={ph} title="pH Air" value={monitoring.PH.toFixed(2)} />
        </div>
      </section>
      <section className="container py-2">
        <div className="pb-2 font-bold px-2">Control</div>
        <div className="flex flex-col gap-4">
          <Card
            logo={relay}
            title="Status Pompa"
            value={monitoring.RELAY === 1 ? "ON" : "OFF"}
          />
        </div>
      </section>
      {/* <section className="container py-2">
        <div className="pb-2 font-bold px-2">Timestamp</div>
        <div className="flex flex-col gap-4">
          <Card
            logo={RELAY}
            title="Timestamp"
            value={
              timestamp[
                Object.keys(timestamp)[Object.keys(timestamp).length - 1]
              ]
            }
          />
        </div>
      </section> */}
    </div>
  );
}
