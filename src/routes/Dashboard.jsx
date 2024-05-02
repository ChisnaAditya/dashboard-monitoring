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
  const [timeEpoch, setTimeEpoch] = useState("");

  const db = database;
  const path = ref(db, "database");

  const convertToDateTime = (timeepoc) => {
    const date = new Date(timeepoc);

    return date;
  };

  useEffect(() => {
    onValue(path, (snapshot) => {
      const data = snapshot.val();
      // console.log(data);
      setReading(data.reading);
      setMonitoring(data.monitoring);
      setTimeEpoch(
        Object.values(data.reading.timestamp)[
          Object.values(data.reading.timestamp).length - 1
        ]
      );
    });
  }, []);

  if (Object.keys(monitoring).length === 0 || Object.keys(reading) === 0) {
    return <p>Loading data...</p>;
  }

  return (
    <div>
      <div className="container flex items-center justify-evenly h-24  rounded-b-[40px] shadow-xl">
        <h1 className="text-4xl text-black font-bold">Dashboard</h1>
      </div>

      <div className="container flex flex-col lg:flex-row gap-4">
        <div className="lg:basis-3/4">
          <section className="py-5">
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
          <section className="py-2">
            <div className="pb-2 font-bold px-2">Control</div>
            <div className="flex flex-col gap-4">
              <Card
                logo={relay}
                title="Status Pompa"
                value={monitoring.RELAY === 1 ? "ON" : "OFF"}
              />
            </div>
          </section>
        </div>
        <div className="lg:basis-1/4">
          <section className="my-5 h-full">
            <h2 className="pb-2 font-bold px-2">User</h2>
            <div className="flex flex-col items-center rounded-3xl shadow-xl bg-pattern p-5 h-[40%]">
              <img
                alt="user image profile"
                src={user}
                width={70}
                height={70}
                className="p-2 bg-white rounded-full"
              />
              <p className="text-xl font-bold mt-5">Naufal Arif</p>
              <p className="">Electronics Engineering</p>
            </div>
            {/* <h2 className="py-2 font-bold px-2">Date & Time</h2>
            <div className="rounded-3xl shadow-xl p-5 h-[40%] bg-gradient-to-r from-indigo-600 to-indigo-500">
              <div className="flex items-center justify-center ">
                <p className="text-white font-thin">
                  {convertToDateTime(timeEpoch).toLocaleString("id-ID")}
                </p>
              </div>
            </div> */}
          </section>
        </div>
      </div>
    </div>
  );
}
