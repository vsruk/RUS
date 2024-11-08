import { User } from "@/typings/user";
import { fetcher } from "./fetcher";
export async function loginMutator<T>(url: string, { arg }: { arg: T }) {
  const response = await fetch(url, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(arg),
  });
  if (!response.ok) {
    throw new Error(`Dogodila se greška kod mutiranja na url: ${url}`);
  }

  const responseData = await response.json();
  /*
  TODO:
  kreirati user objekt
  */
  localStorage.setItem("ezgrada-header", JSON.stringify({}));
  return {
    data: responseData,
  };
}

export async function postMutator<T>(url: string, { arg }: { arg: T }) {
  return await fetcher<{ review: User }>(url, {
    method: "POST",
    body: JSON.stringify(arg),
  });
}

export async function deleteMutator<T>(url: string, { arg }: { arg: T }) {
  return await fetcher<{ review: User }>(url, {
    method: "DELETE",
    body: JSON.stringify(arg),
  });
}

export async function patchMutator<T>(url: string, { arg }: { arg: T }) {
  return await fetcher<{ review: User }>(url, {
    method: "PATCH",
    body: JSON.stringify(arg),
  });
}
