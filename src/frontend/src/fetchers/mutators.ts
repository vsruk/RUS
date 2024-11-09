import { User } from "@/typings/user";
import { fetcher } from "./fetcher";
export async function loginMutator<T>(url: string, { arg }: { arg: T }) {
  const response = await fetch(url, {
    method: "POST",
    headers: {
			Accept: 'application/json',
			'Content-Type': 'application/json',
    },
    body: JSON.stringify(arg),
  });
  if (!response.ok) {
    const error = await response.json();
		throw error;
  }

  const responseData = await response.json();
  
  return {
    ...responseData,
    /* token: response.headers.get("token"),
    role: response.headers.get("role") */
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
