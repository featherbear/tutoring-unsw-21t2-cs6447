// const style = {
//   minWidth: '300px',
//   textAlign: 'center',
//   fontFamily: 'monospace'
// }

window.addEventListener('load', function () {
  function triggerFuzzerAnimation (container) {
    // Based off https://github.com/featherbear/UNSW-COMP6447-ass/blob/www/src/components/home/TitleFuzz.js

    const letterMap = {
      s: 'sƨSƧ5$',
      h: 'h#ʜ',
      e: 'eɘE3',
      l: 'lLๅ1',
      c: 'cC',
      o: 'oO0',
      d: 'dDმ'
    }

    container.dataset.original = container.innerText
    const fuzzMap = container.dataset.original.split('').map(c => letterMap[c])

    const randItem = list => list[Math.floor(Math.random() * list.length)]

    function generateNewString (text) {
      const arr = text.split('')
      const i = Math.floor(Math.random() * arr.length)
      arr[i] = randItem(fuzzMap[i])
      return arr.join('')
    }

    const interval = setInterval(() => {
      container.innerText = generateNewString(container.innerText)
    }, 75)
  }

  for (let elem of document.getElementsByClassName('fuzzTarget'))
    triggerFuzzerAnimation(elem)
})
